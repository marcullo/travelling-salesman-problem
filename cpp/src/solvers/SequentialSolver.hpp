#ifndef TRAVELLING_SALESMAN_PROBLEM_SEQUENTIALSOLVER_H
#define TRAVELLING_SALESMAN_PROBLEM_SEQUENTIALSOLVER_H

#include <algorithm>
#include <numeric>
#include "Solver.hpp"

namespace tsp {

class SequentialSolver : public Solver
{
public:
    SequentialSolver() : Solver(__func__) {}

    /*
     * @brief Solve TSP using sequential approach.
     *
     * @details Let's say (without loosing generality) that 0 is both begin
     *          and end of a trip.
     */
    virtual void solve(const Graph& graph) {
        const auto& costs = graph.getCosts();
        const auto n = graph.getSize();
        std::vector<int> path(n + 1);

        // path: (0, 1, ..., n, 0)
        std::iota(path.begin(), path.end() - 1, 0);
        path[n] = 0;

        do {
            double total_cost = 0.0;

            for (int v = 0; v < n; v++) {
                const int from= path[v];
                const int to = path[v + 1];
                total_cost += costs.at(from).at(to);
            }

            if (total_cost < best_cost) {
                best_path = path;
                best_cost = total_cost;
            }
        // City 0 is always both begin and end of a trip
        } while (std::next_permutation(path.begin() + 1, path.end() - 1));
    }

    virtual void showResult() {
        std::cout << name << std::endl;
        for (const auto& v : best_path) {
            std::cout << v << " ";
        } printf("(%.2lf)", best_cost); std::cout << std::endl;
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_SEQUENTIALSOLVER_H
