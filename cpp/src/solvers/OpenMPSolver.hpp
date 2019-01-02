#ifndef TRAVELLING_SALESMAN_PROBLEM_OPENMPSOLVER_H
#define TRAVELLING_SALESMAN_PROBLEM_OPENMPSOLVER_H

#include "ParallelSolver.hpp"
#include <omp.h>

namespace tsp {

class OpenMPSolver : public ParallelSolver
{
public:
    OpenMPSolver(int threads_nr = 1) : ParallelSolver(__func__, threads_nr) {}

    /*
     * @brief Solve TSP using OpenMP.
     *
     * @details Let's say (without loosing generality) that 0 is both begin
     *          and end of a trip. Let's also say that second city is given
     *          (corresponding to the number of thread. The idea is to assign
     *          independent permutations of n-2 cities for each thread.
     */
    virtual void solve(const Graph& graph) {
        const auto& costs = graph.getCosts();
        const auto n = graph.getSize();
        std::vector<std::vector<int>> best_paths(n - 1);
        std::vector<double> best_costs(n - 1);
        int id;

        for (auto& cost : best_costs) {
            cost = best_cost;
        }

#pragma omp parallel private(id) shared(costs, best_paths, best_costs) num_threads(threads_nr)
#pragma omp for
        for (id = 1; id < n; id++) {
            std::vector<int> path;

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
                    const int from = path[v];
                    const int to = path[v + 1];
                    total_cost += costs.at(from).at(to);
                }

                if (total_cost < best_costs[id - 1]) {
                    best_costs[id - 1] = total_cost;
                    best_paths[id - 1] = path;
                }
            // City 0 is always both begin and end of a trip and city i is the secondly visited one
            } while (std::next_permutation(path.begin() + 2, path.end() - 1));
        }

        int best_id = 0;
        for (int id = 0; id < n - 1; id++) {
            const auto cost = best_costs[id];

            // Select best path
            if (cost < best_cost) {
                best_cost = cost;
                best_id = id;
            }
        }
        best_cost = best_costs[best_id];
        best_path = best_paths[best_id];
    }
};

} // tsp


#endif //TRAVELLING_SALESMAN_PROBLEM_OPENMPSOLVER_H
