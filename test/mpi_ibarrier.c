#include "../dan_mpi.h"
#include <unistd.h>

int main(int argc, char** argv)
{
    dan_mpi_ibarrier ibarrier;
    MPI_Init(&argc,&argv);
    dan_mpi_begin_ibarrier(&ibarrier,0);
    while (!dan_mpi_ibarrier_done(&ibarrier));
    MPI_Finalize();
    return 0;
}
