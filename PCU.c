/*
    Copyright 2012 Dan Ibanez
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    
    http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <string.h>
#include "dan_pmsg.h"
#include "PCU.h"

int PCU_Comm_Init(PCU_Inst* pcu)
{
    dan_pmsg temp = DAN_PMSG_INIT;
    *pcu = dan_malloc(sizeof(*pcu));
    **pcu = temp;
    dan_pmsg_init(*pcu);
    return PCU_SUCCESS;
}

int PCU_Comm_Start(PCU_Inst pcu, PCU_Method method)
{
    dan_pmsg_start(pcu, (dan_pmsg_method)method);
    return PCU_SUCCESS;
}

int PCU_Comm_Pack(PCU_Inst pcu, int to_rank, void* data, size_t size)
{
    memcpy(dan_pmsg_pack(pcu,to_rank,size),data,size);
    return PCU_SUCCESS;
}

int PCU_Comm_Packed(PCU_Inst pcu, int to_rank, size_t* size)
{
    *size = dan_pmsg_packed(pcu,to_rank);
    return PCU_SUCCESS;
}

int PCU_Comm_Send(PCU_Inst pcu)
{
    dan_pmsg_send(pcu);
    return PCU_SUCCESS;
}

int PCU_Comm_Receive(PCU_Inst pcu, bool* done)
{
    *done = dan_pmsg_receive(pcu);
    return PCU_SUCCESS;
}

int PCU_Comm_From(PCU_Inst pcu, int* from_rank)
{
    *from_rank = dan_pmsg_received_from(pcu);
    return PCU_SUCCESS;
}

int PCU_Comm_Received(PCU_Inst pcu, size_t* size)
{
    *size = dan_pmsg_received_size(pcu);
    return PCU_SUCCESS;
}

int PCU_Comm_Unpack(PCU_Inst pcu, void* data, size_t size)
{
    memcpy(data,dan_pmsg_unpack(pcu,size),size);
    return PCU_SUCCESS;
}

int PCU_Comm_Free(PCU_Inst pcu)
{
    dan_pmsg_free(pcu);
    return PCU_SUCCESS;
}

