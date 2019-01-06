#ifndef TRAVELLING_SALESMAN_PROBLEM_RUNNER_H
#define TRAVELLING_SALESMAN_PROBLEM_RUNNER_H

#include <list>
#include <unordered_map>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <memory>
#include "Solver.hpp"
#include "OpenMPTimer.hpp"

namespace tsp
{

class Runner
{
private:
    std::list<Solver*> solvers;
    std::unordered_map<std::string, double> durations;
    std::string reference_name;
    std::unique_ptr<Timer> timer;

public:
    Runner() : timer(std::make_unique<OpenMPTimer>()) {}

    void setReference(Solver& solver) {
        reference_name = solver.getName();
    }

    void addSolver(Solver& solver) {
        solvers.emplace_back(&solver);
    }

    void run(const Graph& graph) {
        for (auto solver : solvers) {
            timer->start();
            solver->solve(graph);
            timer->stop();

            auto duration = timer->getDuration();
            auto name = solver->getName();
            durations[name] = duration;

            solver->showResult();
            showDuration(duration);
        }

        showSummary();
    }

    void profile(const Graph& graph) {
        for (auto solver : solvers) {
            timer->start();
            solver->solve(graph);
            timer->stop();

            auto duration = timer->getDuration();
            auto name = solver->getName();
            durations[name] = duration;

            std::stringstream fileName;
            fileName << "CPP_" << name;
            fileName << "_size_" << std::setfill('0') << std::setw(2) << graph.getSize();
            fileName << "_slots_" << std::setfill('0') << std::setw(2) << solver->getNumberOfThreads();
            fileName << ".txt";

            saveResult(fileName.str(), duration);
#ifdef DEBUG
            solver->showResult();
            showDuration(duration);
#endif
        }
    }

private:
    void showDuration(double d) {
        printf("Time: %.3fs\n", d);
    }

    void showSummary() {
        if (!solvers.size()) {
            return;
        }

        printf("\nSUMMARY\n");

        bool show_speedup = false;

        for (auto solver : solvers) {
            const auto name = solver->getName();
            auto duration = durations[name];

            if (name.compare(reference_name) == 0) {
                printf("%18s: ( reference ) %lf s\n", name.c_str(), duration);
                show_speedup = true;
            } else if (show_speedup) {
                const auto speedup = durations[reference_name] / duration;
                printf("%18s: %.2lf speedup, %.6lf s\n", name.c_str(), speedup, duration);
            } else {
                printf("%18s: %lf s\n", name.c_str(), duration);
            }
        }
    }

    virtual void saveResult(const std::string& fileName, double duration) {
        std::ofstream out{fileName, std::ofstream::app};

        if (!out.is_open()) {
            throw std::invalid_argument("Make sure result name is valid!");
        }

        out << std::fixed << std::setprecision(3) << duration << std::endl;
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_RUNNER_H
