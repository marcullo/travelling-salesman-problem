#include "Graph.hpp"
#include "ComparisonState.hpp"

using tsp::Graph;
using tsp::ComparisonState;

int main(int argc, char* argv[])
{
    const int graphSize = 5;
    const int threadsNr = 4;

    Graph graph{graphSize};
    graph.show();

    ComparisonState state{threadsNr};
    state.run(graph);

    return 0;
}