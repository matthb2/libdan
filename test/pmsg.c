
#include <stdio.h>
#include <assert.h>
#include "../dan_pmsg.h"

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);
    int rank = dan_mpi_rank();
    int size = dan_mpi_size();
    int peer = (rank+1)%size;
    int message = 42;
    dan_pmsg m;
    dan_pmsg_init(&m);
    dan_pmsg_start(&m,dan_pmsg_global);
    DAN_PMSG_PACK(&m,peer,message,int);
    dan_pmsg_send(&m);
    while (dan_pmsg_receive(&m))
    {
        assert(dan_pmsg_received_size(&m) == sizeof(message));
        int* unpacked = dan_pmsg_unpack(&m,sizeof(int));
        fprintf(stderr,"%d received %d from %d\n",
                rank,*unpacked,dan_pmsg_received_from(&m));
        assert(dan_pmsg_received_size(&m) == sizeof(message));
    }
    message = 128;
    fprintf(stderr,"%d stage 2\n",rank);
    dan_pmsg_start(&m,dan_pmsg_global);
    DAN_PMSG_PACK(&m,peer,message,int);
    dan_pmsg_send(&m);
    while (dan_pmsg_receive(&m))
    {
        assert(dan_pmsg_received_size(&m) == sizeof(message));
        int* unpacked = dan_pmsg_unpack(&m,sizeof(int));
        fprintf(stderr,"%d received %d from %d\n",
                rank,*unpacked,dan_pmsg_received_from(&m));
        assert(dan_pmsg_received_size(&m) == sizeof(message));
    }
    int peer2 = (rank+size-1)%size;
    fprintf(stderr,"%d stage 3\n",rank);
    message = 3;
    dan_pmsg_start(&m,dan_pmsg_local);
    DAN_PMSG_PACK(&m,peer,message,int);
    DAN_PMSG_PACK(&m,peer2,message,int);
    dan_pmsg_send(&m);
    while (dan_pmsg_receive(&m))
    {
        assert(dan_pmsg_received_size(&m) == sizeof(message));
        int* unpacked = dan_pmsg_unpack(&m,sizeof(int));
        fprintf(stderr,"%d received %d from %d\n",
                rank,*unpacked,dan_pmsg_received_from(&m));
        assert(dan_pmsg_received_size(&m) == sizeof(message));
    }
    dan_pmsg_free(&m);
    MPI_Finalize();
    return 0;
}
