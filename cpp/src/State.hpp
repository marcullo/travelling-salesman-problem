#ifndef TRAVELLING_SALESMAN_PROBLEM_STATE_H
#define TRAVELLING_SALESMAN_PROBLEM_STATE_H

#include "Graph.hpp"
#include "Runner.hpp"
#include "SequentialSolver.hpp"
#include "OpenMPSolver.hpp"
#include "ThreadSolver.hpp"

namespace tsp
{

class State
{
public:
    virtual void run(const Graph& graph) = 0;
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_STATE_H
