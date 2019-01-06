#ifndef TRAVELLING_SALESMAN_PROBLEM_MPILOADER_H
#define TRAVELLING_SALESMAN_PROBLEM_MPILOADER_H

#include <memory>
#include "Graph.hpp"

namespace tsp
{

class MPILoader
{
public:
    std::shared_ptr<Graph> load(int argc, char* argv[]) {
        argc--;
        argv++;

        int graph_size;
        std::string graph_file_name;

        switch(argc)
        {
            case 0:
                return std::make_shared<Graph>(5);

            case 1:
                graph_file_name = argv[0];
                return std::make_shared<Graph>(graph_file_name);

            case 4:
            case 5: {
                bool generated = static_cast<bool>(std::stoi(argv[0]));

                if (!generated && argc == 4) {
                    throw std::invalid_argument("You should provide an input file for graph as a last argument!");
                }

                graph_size = std::stoi(argv[1]);

                if (!graph_size) {
                    throw std::invalid_argument("Invalid graph size!");
                }

                // argv[2] is dummy to provide compatibility with raw cpp solution

                std::shared_ptr<Graph> graph;

                if (argc == 5) {
                    graph_file_name = argv[4];
                    graph = std::make_shared<Graph>(graph_file_name);
                } else {
                    graph = std::make_shared<Graph>(graph_size);
                }

                // Some cases are reserved by cpp/src/Loader.hpp.
                // To provide compatibility with raw cpp solution, we need to keep numbering.
                if (std::atoi(argv[3]) == 3) {
                    return graph;
                } else {
                    throw std::invalid_argument("Invalid solver for MPI Loader!");
                }
            }

            default:
                throw std::invalid_argument("Invalid input set!");
        }
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_MPILOADER_H
