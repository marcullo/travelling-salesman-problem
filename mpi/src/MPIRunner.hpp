#ifndef TRAVELLING_SALESMAN_PROBLEM_MPIRUNNER_H
#define TRAVELLING_SALESMAN_PROBLEM_MPIRUNNER_H

#include <memory>
#include <sstream>
#include <iomanip>
#include "MPITimer.hpp"
#include "MPISolver.hpp"

namespace tsp
{

class MPIRunner
{
    MPISolver* solver;
    std::unique_ptr<Timer> timer;

public:
    MPIRunner(MPISolver* solver) : solver(solver), timer(std::make_unique<MPITimer>()) {}

    void run(const Graph& graph) {
        timer->start();
        solver->solve(graph);
        timer->stop();

        if (!solver->isRoot()) {
            return;
        }

        const auto duration = timer->getDuration();
        const auto solverName = solver->getName();
        const auto graphSize = graph.getSize();
        const auto generated = graph.isGenerated();

        auto filename = generateFileName(solverName, graphSize, generated);
        saveResult(filename, duration);
#ifdef DEBUG
        solver->showResult();
        showDuration(duration);
#endif
    }

private:
    void showDuration(double d) {
        printf("Time: %.3fs\n", d);
    }

    std::string generateFileName(const std::string& solverName, int graphSize, bool generated) {
        std::stringstream fileName;

        fileName << "CPP_" << solverName;
        fileName << "_size_" << std::setfill('0') << std::setw(2) << graphSize;
        fileName << "_slots_" << std::setfill('0') << std::setw(2) << solver->getNumberOfProcesses();
        fileName << ".txt";

        return fileName.str();
    }

    void saveResult(const std::string& fileName, double duration) {
        std::ofstream out{fileName, std::ofstream::app};

        if (!out.is_open()) {
            throw std::invalid_argument("Make sure result name is valid!");
        }

        out << std::fixed << std::setprecision(3) << duration << std::endl;
    }
};

} //tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_MPIRUNNER_H
