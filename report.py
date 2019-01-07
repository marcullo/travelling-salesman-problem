import argparse
import sys
import os
from functools import reduce


def log(x):
    print(x)
    sys.stdout.flush()
    sys.stdout.flush()


def log_error(x):
    log('Error: ' + x)


def exit_script(ret):
    sys.exit(ret)


def get_average(lst):
    return reduce(lambda a, b: a + b, lst) / len(lst)


def get_samples(name, folder, max_rows):
    lst = []
    path = '{0}/{1}'.format(folder, name)
    with open(path, 'r') as f:
        for i in range(max_rows):
            line = f.readline()
            if not line:
                break
            lst.append(line)
        return list(map(float, lst))


class Result:
    def __init__(self, name, value):
        self.content = {}

        tokens = name[:-len('.txt')].split('_')

        self.content['value'] = value
        self.content['lang'] = tokens[0]
        self.content['solver'] = tokens[1]

        for i in range(2, len(tokens), 2):
            key = tokens[i]
            val = int(tokens[i + 1])
            self.content[key] = val

        if 'slots' not in self.content:
            self.content['slots'] = 1
        if 'size' not in self.content:
            raise ValueError

    @staticmethod
    def get_header():
        return 'size;slots;solver;value;lang\n'

    def get_string(self):
        return '{0};{1};{2};{3:.3f};{4}\n'.format(self.content['size'], self.content['slots'],
                                                  self.content['solver'], self.content['value'],
                                                  self.content['lang'])


def get_results(args):
    results = []
    result_names = list(filter(lambda x: x.endswith(".txt"), os.listdir(args.folder)))
    for name in result_names:
        print('Reading {0}'.format(name))
        try:
            samples = get_samples(name, args.folder, args.max_rows)
            duration = get_average(samples)
            res = Result(name, duration)
        except ValueError:
            log_error('Invalid file content!')
            continue
        results.append(res)
    print('Processed {0}/{1} .txt files successfully'.format(len(results), len(result_names)))
    return results


def save_results(results, file):
    print('Saving to {0}'.format(file))
    with open(file, 'w') as f:
        f.write(Result.get_header())
        for res in results:
            f.write(res.get_string())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--folder', type=str, required=True,
                        help='results folder')
    parser.add_argument('-o', '--output', type=str, default='summary.csv',
                        help='output file name (default summary.csv)')
    parser.add_argument('-max', '--max_rows', type=int, default=20,
                        help='max number of rows to process in each file')
    parser_args = parser.parse_args()

    if not os.path.isdir(parser_args.folder):
        log_error('Invalid results folder!')
        exit_script(1)

    if parser_args.max_rows < 1:
        log_error('Invalid rows number!')
        exit_script(1)

    return parser_args


def main():
    try:
        args = parse_args()
        results = get_results(args)
        save_results(results, args.output)
    except (KeyboardInterrupt, SystemExit):
        pass


main()
