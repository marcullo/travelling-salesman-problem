#include <iostream>
#include "MPISolver.hpp"

using tsp::MPISolver;
using tsp::Graph;

int main(int argc, char* argv[])
{
    try {
        Graph graph{5};
        MPISolver solver;

        graph.show();
        solver.solve(graph);

        if (solver.isRoot()) {
            graph.show();
            solver.showResult();
        }
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

