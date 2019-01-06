#ifndef TRAVELLING_SALESMAN_PROBLEM_MPITIMER_H
#define TRAVELLING_SALESMAN_PROBLEM_MPITIMER_H

#include <mpi.h>

#include "Timer.hpp"

namespace tsp
{

class MPITimer : public Timer
{
public:
    virtual void start() {
        start_time = MPI_Wtime();
        stop_time = 0.0;
    }

    virtual void stop() {
        stop_time = MPI_Wtime();
    }
};

} // tsp


#endif //TRAVELLING_SALESMAN_PROBLEM_MPITIMER_H
