
#include <stdio.h>
#include <assert.h>
#include "../dan_pmsg.h"

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);
    dan_pmsg pmsg;
    dan_pmsg_init(&pmsg);
    dan_pmsg_start(&pmsg,dan_pmsg_global);
    int rank = dan_mpi_rank();
    int size = dan_mpi_size();
    int message = 42;
    int peer = (rank+1)%size;
    DAN_PMSG_RESERVE(&pmsg,peer,message);
    assert(sizeof(message)==dan_pmsg_reserved(&pmsg,peer));
    dan_pmsg_begin_packing(&pmsg);
    DAN_PMSG_PACK(&pmsg,peer,message,int);
    dan_pmsg_send(&pmsg);
    while (dan_pmsg_receive(&pmsg))
    {
        fprintf(stderr,"%d received %d from %d\n",
                rank,*((int*)pmsg.received.buffer.data),
                pmsg.received.peer);
    }
    dan_pmsg_start(&pmsg,dan_pmsg_global);
    fprintf(stderr,"%d step 2\n",rank);
    message = 128;
    peer = (rank+(size-1))%size;
    DAN_PMSG_RESERVE(&pmsg,peer,message);
    assert(sizeof(message)==dan_pmsg_reserved(&pmsg,peer));
    dan_pmsg_begin_packing(&pmsg);
    DAN_PMSG_PACK(&pmsg,peer,message,int);
    dan_pmsg_send(&pmsg);
    while (dan_pmsg_receive(&pmsg))
    {
        fprintf(stderr,"%d received %d from %d\n",
                rank,*((int*)pmsg.received.buffer.data),
                pmsg.received.peer);
    }
    dan_pmsg_free(&pmsg);
    MPI_Finalize();
    return 0;
}
