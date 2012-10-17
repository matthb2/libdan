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

#ifndef PCU_H
#define PCU_H

#define PCU_SUCCESS 0
#define PCU_FAILURE -1

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#ifdef __cplusplus
enum PCU_Method { PCU_GLOBAL_METHOD, PCU_LOCAL_METHOD };
#else
typedef enum { PCU_GLOBAL_METHOD, PCU_LOCAL_METHOD } PCU_Method;
#endif
typedef dan_pmsg* PCU_Inst;

int PCU_Comm_Init(PCU_Inst* pcu);
int PCU_Comm_Start(PCU_Inst pcu, PCU_Method method);
int PCU_Comm_Pack(PCU_Inst pcu, int to_rank, void* data, size_t size);
#define PCU_COMM_PACK(pcu,to_rank,object)\
PCU_Comm_Pack(pcu,to_rank,&object,sizeof(object))
int PCU_Comm_Packed(PCU_Inst pcu, int to_rank, size_t* size);
int PCU_Comm_Send(PCU_Inst pcu);
int PCU_Comm_Receive(PCU_Inst pcu, bool* done);
int PCU_Comm_From(PCU_Inst pcu, int* from_rank);
int PCU_Comm_Received(PCU_Inst pcu, size_t* size);
int PCU_Comm_Unpack(PCU_Inst pcu, void* data, size_t size);
#define PCU_COMM_UNPACK(pcu,object)\
PCU_Comm_Unpack(pcu,&object,sizeof(object))
int PCU_Comm_Free(PCU_Inst pcu);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
