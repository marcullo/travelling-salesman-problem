#include <iostream>
#include "MPISolver.hpp"
#include "MPILoader.hpp"
#include "MPIRunner.hpp"

using tsp::MPILoader;
using tsp::MPISolver;
using tsp::MPIRunner;

int main(int argc, char* argv[])
{
    try {
        MPILoader loader;
        MPISolver solver;
        MPIRunner runner{&solver};

        const auto graph = loader.load(argc, argv);
        runner.run(*graph);
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

