#include "Graph.hpp"
#include "Runner.hpp"
#include "SequentialSolver.hpp"

using tsp::Graph;
using tsp::Runner;
using tsp::SequentialSolver;

int main(int argc, char* argv[])
{
    const int graphSize = 5;

    Graph graph{graphSize};
    graph.show();

    Runner runner;
    SequentialSolver sequentialSolver;

    runner.addSolver(sequentialSolver);

    runner.run(graph);

    return 0;
}