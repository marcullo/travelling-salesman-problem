#include "Graph.hpp"
#include "Runner.hpp"
#include "SequentialSolver.hpp"
#include "OpenMPSolver.hpp"

using tsp::Graph;
using tsp::Runner;
using tsp::SequentialSolver;
using tsp::OpenMPSolver;

int main(int argc, char* argv[])
{
    const int graphSize = 5;
    const int threadsNr = 4;

    Graph graph{graphSize};
    graph.show();

    Runner runner;
    SequentialSolver sequentialSolver;
    OpenMPSolver openMPSolver{threadsNr};

    runner.addSolver(sequentialSolver);
    runner.addSolver(openMPSolver);

    runner.setReference(sequentialSolver);
    runner.run(graph);

    return 0;
}