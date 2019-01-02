#ifndef TRAVELLING_SALESMAN_PROBLEM_COMPARISONSTATE_H
#define TRAVELLING_SALESMAN_PROBLEM_COMPARISONSTATE_H

#include "State.hpp"
#include "Graph.hpp"
#include "Runner.hpp"
#include "SequentialSolver.hpp"
#include "OpenMPSolver.hpp"
#include "ThreadSolver.hpp"

namespace tsp
{

class ComparisonState : public State
{
private:
    int threads_nr;

public:
    ComparisonState(int threads_nr = 4) : State(), threads_nr(threads_nr) {}

    virtual void run(const Graph& graph) {
        SequentialSolver sequentialSolver;
        OpenMPSolver openMPSolver{threads_nr};
        ThreadSolver threadSolver{threads_nr};

        graph.show();

        Runner runner;
        runner.addSolver(sequentialSolver);
        runner.addSolver(openMPSolver);
        runner.addSolver(threadSolver);

        runner.setReference(sequentialSolver);
        runner.run(graph);
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_COMPARISONSTATE_H
