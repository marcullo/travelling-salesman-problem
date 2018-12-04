import argparse
import sys
import os
import random
from subprocess import Popen

MODES = ['debug', 'release']
LANGUAGES = ['c', 'cpp']
SOLVERS = ['seq', 'omp', 'thd', 'mpi', 'all']


def log(x):
    print(x)
    sys.stdout.flush()
    sys.stdout.flush()


def log_error(x):
    log('Error: ' + x)


def exit_script(ret):
    sys.exit(ret)


def generate_graph(size, filename, path):
    graph = ''
    file_path = '{0}/{1}'.format(path, filename)
    with open(file_path, 'w') as f:
        f.write('{0}\n'.format(size))
        for i in range(size):
            line = get_random_line(size, i)
            f.write(line)
            graph += line
    return graph


def get_random_line(n, zero_index):
    line = ''
    for i in range(n):
        if i == zero_index:
            line += '0.0 ' # path to itself
        else:
            line += '{0:.2f} '.format(get_random_cost())
    line += '\n'
    return line


def get_random_cost():
    return random.random() * 10.0


class Environment:
    def __init__(self, args):
        self.args = args
        self.generated = 1 if args.input is None else 0
        self.target_folder = ''
        self.target_name = ''
        self.results_folder = 'tests'
        self.inputs_folder = '../input'
        self.graph_gen_file = '.generated.txt'

        if self.args.clean:
            files_to_remove = os.listdir(self.results_folder)
            for name in files_to_remove:
                if name.endswith(".txt"):
                    os.remove(os.path.join(self.results_folder, name))

    def __del__(self):
        file_path = '{0}/{1}'.format(self.inputs_folder, self.graph_gen_file)
        if os.path.exists(file_path):
            os.remove(file_path)

    def open_results_folder(self):
        if self.args.verbose:
            print('Running from: {0}/'.format(self.results_folder))
        os.chdir(self.results_folder)

    def nothing_to_do(self):
        return self.args.iterations is 0 and self.args.verbose

    def prepare_graph(self):
        if self.args.input is None:
            if self.args.verbose and self.args.mode == 'debug':
                print('Generating cost matrix...')

            self.args.input = self.graph_gen_file
            graph = generate_graph(self.args.cities, self.args.input, self.inputs_folder)

            if self.args.verbose and self.args.mode == 'debug':
                print(graph)

    def get_command(self, solver):
        lang = self.args.lang if solver != 'mpi' else 'mpi'
        self.target_folder = '../cmake-build-{0}/{1}'.format(self.args.mode, lang)
        self.target_name = '{0}/solver_{1}'.format(self.target_folder, lang)
        if os.name == 'nt':
            self.target_name += '.exe'

        command = '"mpiexec" -n {0} '.format(self.args.slots) if solver == 'mpi' else ''
        command += '{0} {1} {2} {3} {4} {5}/{6}'.format(self.target_name, self.generated,
                                                        self.args.cities, self.args.slots,
                                                        SOLVERS.index(solver), self.inputs_folder,
                                                        self.args.input)

        if self.args.verbose:
            if self.args.iterations < 2:
                print('Run: \'{0}\''.format(command))
            else:
                print('Run ({1} times): \'{0}\''.format(command, self.args.iterations))

        return command

    def get_iterations(self):
        return range(self.args.iterations)

    def get_solvers(self):
        if self.args.type == 'all':
            if self.args.lang == 'cpp':
                return SOLVERS[:-1]
            else:
                return SOLVERS[:-2]
        else:
            return [self.args.type]


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-cln', '--clean', action='store_true',
                        help='remove all previous results')
    parser.add_argument('-m', '--mode', type=str, choices=MODES, default='debug',
                        help='app mode (default debug)')
    parser.add_argument('-l', '--lang', type=str, choices=LANGUAGES, default='cpp',
                        help='language of solution to profile (default cpp)')
    parser.add_argument('-i', '--iterations', type=int, default=1,
                        help='number of iterations (default 1)')
    parser.add_argument('-t', '--type', type=str, choices=SOLVERS, default='seq',
                        help='solver type (default seq)')
    parser.add_argument('-s', '--slots', type=int, default=2,
                        help='num of slots: processes or threads, depending on context (default 2)')
    parser.add_argument('-n', '--cities', type=int, default=10,
                        help='number of cities (default 10)')
    parser.add_argument('-in', '--input', type=str, default=None,
                        help='costs matrix file to be read from "input" folder (if {0}, '
                             'costs will be generated with random numbers)'.format(None))
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='print something')
    parser_args = parser.parse_args()

    if parser_args.cities < 2:
        log_error('Number of cities should be greater than 0')
        exit_script(1)

    if parser_args.verbose:
        if parser_args.type == 'mpi' or parser_args.type == 'all':
            if parser_args.lang != 'cpp':
                print('Warning: mpi works only for cpp')

    if parser_args.type == 'seq':
        parser_args.slots = 2
    elif parser_args.slots < 2:
        log_error('Slots nr should be greater than 1')
        exit_script(1)
    elif parser_args.slots >= parser_args.cities:
        log_error('Slots nr should be lower that number of cities')
        exit_script(1)

    return parser_args


def run(env, solver):
    command = env.get_command(solver)
    for _ in env.get_iterations():
        p = Popen(command, shell=True)
        p.communicate()
    return


def main():
    try:
        args = parse_args()
        env = Environment(args)

        env.open_results_folder()
        if env.nothing_to_do():
            print('Nothing to be done')
            return

        env.prepare_graph()

        for solver in env.get_solvers():
            run(env, solver)
    except (KeyboardInterrupt, SystemExit):
        pass

    exit_script(0)


main()
