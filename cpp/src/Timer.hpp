#ifndef TRAVELLING_SALESMAN_PROBLEM_TIMER_H
#define TRAVELLING_SALESMAN_PROBLEM_TIMER_H

namespace tsp
{

class Timer
{
protected:
    double start_time;
    double stop_time;

public:
    Timer() : start_time(0.0), stop_time(0.0) {}

    virtual void start() = 0;

    virtual void stop() = 0;

    double getDuration() {
        return stop_time - start_time;
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_TIMER_H
