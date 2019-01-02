#ifndef TRAVELLING_SALESMAN_PROBLEM_DURATIONTESTSTATE_H
#define TRAVELLING_SALESMAN_PROBLEM_DURATIONTESTSTATE_H

#include "State.hpp"

namespace tsp
{

class DurationTestState : public State
{
    std::shared_ptr<Solver> solverUnderTest;

public:
    DurationTestState(std::shared_ptr<Solver> solver) : State(), solverUnderTest(solver) {}

    virtual void run(const Graph& graph) {
        Runner runner;
        runner.addSolver(*solverUnderTest);
        runner.profile(graph);
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_DURATIONTESTSTATE_H
