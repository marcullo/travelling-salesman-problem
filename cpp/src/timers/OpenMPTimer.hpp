#ifndef TRAVELLING_SALESMAN_PROBLEM_OPENMPTIMER_H
#define TRAVELLING_SALESMAN_PROBLEM_OPENMPTIMER_H

#include <omp.h>

#include "Timer.hpp"

namespace tsp
{

class OpenMPTimer : public Timer
{
public:
    virtual void start() {
        start_time = omp_get_wtime();
        stop_time = 0.0;
    }

    virtual void stop() {
        stop_time = omp_get_wtime();
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_OPENMPTIMER_H
