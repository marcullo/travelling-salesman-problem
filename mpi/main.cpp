#include <iostream>
#include "MPISolver.hpp"
#include "MPILoader.hpp"

using tsp::MPILoader;
using tsp::MPISolver;

int main(int argc, char* argv[])
{
    try {
        MPILoader loader;
        MPISolver solver;

        const auto graph = loader.load(argc, argv);
        solver.solve(*graph);

        if (solver.isRoot()) {
            graph->show();
            solver.showResult();
        }
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

