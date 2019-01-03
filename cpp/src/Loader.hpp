#ifndef TRAVELLING_SALESMAN_PROBLEM_LOADER_H
#define TRAVELLING_SALESMAN_PROBLEM_LOADER_H

#include <memory>
#include <string>
#include <stdexcept>
#include "ComparisonState.hpp"
#include "DurationTestState.hpp"

namespace tsp
{

class Loader
{
public:
    std::pair<std::shared_ptr<State>, std::shared_ptr<Graph>> load(int argc, char *argv[]) {
        argc--;
        argv++;

        int graph_size;
        int threads_nr;
        std::string graph_file_name;

        switch (argc)
        {
            case 0:
                return std::make_pair(std::make_shared<ComparisonState>(), std::make_shared<Graph>(5));

            case 1:
                graph_file_name = argv[0];
                return std::make_pair(std::make_shared<ComparisonState>(), std::make_shared<Graph>(graph_file_name));

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

                threads_nr = std::stoi(argv[2]);

                if (threads_nr < 2) {
                    throw std::invalid_argument("Invalid threads number. Should be grater than 1!");
                }


                std::shared_ptr<Graph> graph;

                if (argc == 5) {
                    graph_file_name = argv[4];
                    graph = std::make_shared<Graph>(graph_file_name);
                } else {
                    graph = std::make_shared<Graph>(graph_size);
                }

                switch (std::atoi(argv[3])) {
                    case 0:
                        return std::make_pair(std::make_shared<DurationTestState>(std::make_shared<SequentialSolver>()), graph);

                    case 1:
                        return std::make_pair(std::make_shared<DurationTestState>(std::make_shared<ThreadSolver>(threads_nr)), graph);

                    case 2:
                        return std::make_pair(std::make_shared<DurationTestState>(std::make_shared<OpenMPSolver>(threads_nr)), graph);

                    default:
                        throw std::invalid_argument("Invalid solver type!");
                }
            }

            default:
                throw std::invalid_argument("Invalid input set!");
        }
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_LOADER_H
