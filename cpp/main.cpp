#include "Loader.hpp"

using tsp::Loader;

int main(int argc, char* argv[])
{
    Loader loader;
    const auto& [state, graph] = loader.load(argc, argv);
    state->run(*graph);

    return 0;
}