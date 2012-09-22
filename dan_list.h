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

#ifndef DAN_LIST_H
#define DAN_LIST_H

struct dan_list_node_struct
{
    struct dan_list_node_struct* next;
};
typedef struct dan_list_node_struct dan_list_node;

#define DAN_LIST_NODE_INIT { .next = 0 }

typedef struct
{
    dan_list_node head;
} dan_list;

#define DAN_LIST_INIT { .head = DAN_LIST_NODE_INIT }

typedef struct
{
    dan_list_node node;
    dan_list_node* previous;
} dan_list2_node;

#define DAN_LIST2_NODE_INIT { .node = DAN_LIST_NODE_INIT, .previous = 0 }

typedef dan_list dan_list2;

#define DAN_LIST2_INIT DAN_LIST_INIT

typedef dan_list2_node dan_list3_node;

#define DAN_LIST3_NODE_INIT DAN_LIST2_NODE_INIT

typedef dan_list3_node dan_list3;

void dan_insert_list_node(dan_list_node* previous, dan_list_node* node);
void dan_remove_list_node(dan_list_node* previous);
void dan_push_list_node(dan_list* list, dan_list_node* node);
void dan_pop_list_node(dan_list* list);
void dan_insert_list2_node(dan_list_node* previous, dan_list2_node* node);
void dan_remove_list2_node(dan_list2_node* node);
void dan_push_list2_node(dan_list2* list, dan_list2_node* node);
void dan_pop_list2_node(dan_list2* list);
dan_list_node* dan_first_list_node(dan_list* list);
dan_list2_node* dan_first_list2_node(dan_list2* list);
dan_list2_node* dan_next_list2_node(dan_list2_node* node);
void dan_list3_init(dan_list3* list);
dan_list3_node* dan_first_list3_node(dan_list3* list);
dan_list3_node* dan_list3_end(dan_list3* list);
dan_list3_node* dan_next_list3_node(dan_list3_node* node);

#endif
