#include "Graph.hpp"

using tsp::Graph;

int main(int argc, char* argv[])
{
    const int graphSize = 5;

    Graph graph{graphSize};
    graph.show();

    return 0;
}