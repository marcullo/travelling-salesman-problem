#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int world_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::cout << world_rank << ": Hello, MPI world!" << std::endl;
    MPI_Finalize();
    return 0;
}

