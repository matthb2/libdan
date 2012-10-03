
#include <stdio.h>
#include "../dan_bsp.h"

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);
    dan_bsp bsp = DAN_BSP_INIT;
    dan_bsp_init(&bsp,0,1);
    dan_bsp_begin_superstep(&bsp);
    int rank = dan_mpi_rank();
    int size = dan_mpi_size();
    int message = 42;
    int peer = (rank+1)%size;
    dan_bsp_reserve(&bsp,peer,sizeof(message));
    dan_bsp_allocate(&bsp);
    dan_bsp_begin_packing(&bsp);
    int* packed = dan_bsp_pack(&bsp,peer,sizeof(message));
    *packed = message;
    dan_bsp_send(&bsp);
    while (dan_bsp_receive(&bsp))
    {
        fprintf(stderr,"%d received %d from %d\n",
                rank,*((int*)bsp.received.buffer.data),
                bsp.received.peer);
    }
    dan_bsp_begin_superstep(&bsp);
    fprintf(stderr,"%d step 2\n",rank);
    message = 128;
    peer = (rank+(size-1))%size;
    dan_bsp_reserve(&bsp,peer,sizeof(message));
    dan_bsp_allocate(&bsp);
    dan_bsp_begin_packing(&bsp);
    packed = dan_bsp_pack(&bsp,peer,sizeof(message));
    *packed = message;
    dan_bsp_send(&bsp);
    while (dan_bsp_receive(&bsp))
    {
        fprintf(stderr,"%d received %d from %d\n",
                rank,*((int*)bsp.received.buffer.data),
                bsp.received.peer);
    }
    dan_bsp_free(&bsp);
    MPI_Finalize();
    return 0;
}
