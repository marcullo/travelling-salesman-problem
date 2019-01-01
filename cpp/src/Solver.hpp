#ifndef TRAVELLING_SALESMAN_PROBLEM_SOLVER_H
#define TRAVELLING_SALESMAN_PROBLEM_SOLVER_H

#include <string>
#include <vector>
#include <limits>
#include <cstdio>
#include "Graph.hpp"

namespace tsp
{

class Solver
{
protected:
    std::string name;
    int threads_nr;
    std::vector<int> best_path;
    double best_cost;

public:
    Solver(std::string name) : name(name), threads_nr(1), best_cost(std::numeric_limits<double>::max()) {}

    virtual void solve(const Graph& graph) = 0;

    virtual void showResult() {}

    int getNumberOfThreads() {
        return threads_nr;
    }

    std::string getName() {
        return name;
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_SOLVER_H
