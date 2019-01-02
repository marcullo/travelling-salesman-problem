#ifndef TRAVELLING_SALESMAN_PROBLEM_PARALLELSOLVER_H
#define TRAVELLING_SALESMAN_PROBLEM_PARALLELSOLVER_H

#include <stdexcept>
#include "Solver.hpp"

namespace tsp
{

class ParallelSolver : public Solver
{
public:
    ParallelSolver(std::string name, int threads_nr = 1) : Solver(name) {
        this->threads_nr = threads_nr;

        if (threads_nr < 1) {
            throw std::invalid_argument("Number of threads should be greater or equal to 1");
        }
    }

    virtual void showResult() {
        std::cout << name << " (threads: " << threads_nr << ")" << std::endl;
        for (const auto& v : best_path) {
            std::cout << v << " ";
        } printf("(%.2lf)", best_cost); std::cout << std::endl;
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_PARALLELSOLVER_H
