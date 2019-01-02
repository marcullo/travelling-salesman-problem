#ifndef TRAVELLING_SALESMAN_PROBLEM_THREADSOLVER_H
#define TRAVELLING_SALESMAN_PROBLEM_THREADSOLVER_H

#include <thread>
#include <cmath>
#include "ParallelSolver.hpp"

namespace tsp {

class ThreadSolver : public ParallelSolver
{
public:
    ThreadSolver(int threads_nr = 1) : ParallelSolver(__func__, threads_nr) {}

    /*
     * @brief Solve TSP using thread.
     *
     * @details Let's say (without loosing generality) that 0 is both begin
     *          and end of a trip. Let's also say that second city is given
     *          (corresponding to the number of thread. The idea is to assign
     *          independent permutations of n-2 cities for each thread.
     */
    virtual void solve(const Graph& graph) {
        const auto n = graph.getSize();
        const auto calculators = threads_nr - 1;
        const int laps = static_cast<const int>(std::ceil(double(n - 1) / calculators));
        std::vector<std::vector<int>> best_paths(n - 1);
        std::vector<double> best_costs(n - 1);

        for (auto& cost: best_costs) {
            cost = best_cost;
        }

        for (int i = 0; i < laps; i++) {
            std::vector<std::thread> threads;

            // Spawn threads
            for (int j = 1; j < calculators + 1; j++) {
                const int id = i * calculators + j;
                if (id > n - 1) {
                    break;
                }
                threads.emplace_back(&ThreadSolver::updateBestPath, this, id, graph,
                        std::ref(best_paths[id - 1]), std::ref(best_costs[id - 1]));
            }

            // Join threads
            for (auto& thread: threads) {
                thread.join();
            }
        }

        int best_i = 0;
        for (int i = 0; i < n - 1; i++) {
            const auto cost = best_costs[i];

            // Select best path id
            if (cost < best_cost) {
                best_cost = cost;
                best_i = i;
            }
        }
        best_cost = best_costs[best_i];
        best_path = best_paths[best_i];
    }

private:
    void updateBestPath(int id, const Graph& graph, std::vector<int>& best_path, double& best_cost) {
        const auto& costs = graph.getCosts();
        const auto n = graph.getSize();
        std::vector<int> path;

        path.push_back(0);
        path.push_back(id);
        for (int v = 1; v < n; v++) {
            // the id-th city is reserved as being secondly visited
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

            if (total_cost < best_cost) {
                best_path = path;
                best_cost = total_cost;
            }

            // City 0 is always both begin and end of a trip and city id is the secondly visited one
        } while (std::next_permutation(path.begin() + 2, path.end() - 1));
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_THREADSOLVER_H
