#ifndef TRAVELLING_SALESMAN_PROBLEM_MPISOLVER_H
#define TRAVELLING_SALESMAN_PROBLEM_MPISOLVER_H

#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "ParallelSolver.hpp"
#include "Graph.hpp"

namespace tsp
{

class MPISolver : public ParallelSolver
{
private:
    static const int root_rank = 0;
    int world_size;
    int world_rank;

public:
    /*
     * @brief Construct solver.
     *
     * @details It should be created only once!
     */
    MPISolver() : ParallelSolver(__func__) {
        MPI_Init(NULL, NULL);
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        if (world_size < 2) {
            throw std::invalid_argument(
                    "You need to provide at least 2 slots: one for manager and one for calculator!");
        }
    }

    ~MPISolver() {
        MPI_Finalize();
    }

    /*
     * @brief Solve TSP using MPI.
     *
     * @details Let's say (without loosing generality) that 0 is both begin
     *          and end of a trip. Let's also say that second city is given
     *          (corresponding to the number of slots). The idea is to assign
     *          independent permutations of n-2 cities for each slot.
     */
    void solve(const Graph& graph) {
        const auto n = graph.getSize();
        const auto second_cities = n - 1;
        const auto best_path_len = n + 1;
        const auto laps_nr = static_cast<unsigned int>(std::ceil(double(second_cities) / (world_size)));
        std::vector<std::vector<int>> laps(laps_nr);

        if (isRoot()) { // manager
            // Prepare ids for of second cities for every process in each lap
            initializeLaps(laps, second_cities);
        }

        for (const auto& lap: laps) {
            std::vector<double> best_costs;
            std::vector<int> best_paths;
            int id;

            // Assign tasks (associated with ids of secondly visited cities)
            MPI_Scatter(lap.data(), 1, MPI_INT, &id, 1, MPI_INT, root_rank, MPI_COMM_WORLD);

            if (isRoot()) { // manager
                manageBuffersResize(best_costs, best_paths, best_path_len);
            }

            // Calculate partial best paths
            calculateBestPath(graph, id);

            // Wait for processes to be ready
            MPI_Barrier(MPI_COMM_WORLD);

            // Gather partial best costs
            MPI_Gather(&best_cost, 1, MPI_DOUBLE,
                       best_costs.data(), 1, MPI_DOUBLE, root_rank, MPI_COMM_WORLD);

            MPI_Barrier(MPI_COMM_WORLD);

            // Gather partial best paths
            MPI_Gather(best_path.data(), best_path.size(), MPI_INT,
                       best_paths.data(), best_path.size(), MPI_INT, root_rank, MPI_COMM_WORLD);

            if (isRoot()) { // manager
                updateBestPath(best_costs, best_paths, best_path_len);
            } else { // calculator
                // Do nothing
            }

            // Wait for processes to be ready
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }

    int getNumberOfProcesses() {
        return world_size;
    }

    bool isRoot() {
        return world_rank == root_rank;
    }

    virtual void showResult() {
        std::cout << name << " (nodes: " << world_size << ")" << std::endl;
        for (const auto& v : best_path) {
            std::cout << v << " ";
        } printf("(%.2lf)", best_cost); std::cout << std::endl;
    }

private:
    void initializeLaps(std::vector<std::vector<int>>& laps, const int last_id) {
        for (size_t i = 0; i < laps.size(); i++) {
            for (int j = 1; j <= world_size; j++) {
                auto& lap = laps[i];
                const int id = i * world_size + j;
                if (id <= last_id) {
                    lap.push_back(id);
                } else {
                    lap.push_back(0); // 0 = do nothing
                }
            }
        }
    }

    void manageBuffersResize(std::vector<double>& best_costs, std::vector<int>& best_paths, size_t best_path_len) {
        best_paths.resize(world_size * best_path_len);
        best_costs.resize(world_size);
    }

    void calculateBestPath(const Graph& graph, const int id) {
        if (!id) {
            // Do nothing because we're done
            return;
        }

        const auto& costs = graph.getCosts();
        const auto n = graph.getSize();
        std::vector<int> path;

        path.clear();
        path.push_back(0);
        path.push_back(id);

        for (int v = 1; v < n; v++) {
            // the id-th city is reserved as being second visited
            if (v == id) {
                continue;
            }
            path.push_back(v);
        }
        path.push_back(0);

        do {
            double total_cost = 0.0;

            for (int v = 0; v < n; v++) {
                const int i = path[v];
                const int j = path[v + 1];
                total_cost += costs.at(i).at(j);
            }

            if (total_cost < best_cost) {
                best_path = path;
                best_cost = total_cost;
            }

            // City 0 is always both begin and end of a trip and city 'id' is the secondly visited one
        } while (std::next_permutation(path.begin() + 2, path.end() - 1));
    }

    void updateBestPath(const std::vector<double>& best_costs, const std::vector<int>& best_paths,
                        size_t best_path_len) {
        best_path.clear();

        int best_cost_i = 0;
        for (size_t i = 0; i < best_costs.size(); i++) {
            double cost = best_costs[i];
            if (cost < best_cost) {
                best_cost = cost;
                best_cost_i = i;
            }
        }
        best_cost_i *= best_path_len;

        for (size_t i = best_cost_i; i < best_cost_i + best_path_len; i++) {
            best_path.push_back(best_paths[i]);
        }
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_MPISOLVER_H
