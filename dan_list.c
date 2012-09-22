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

#include "dan_list.h"

void dan_insert_list_node(dan_list_node* previous, dan_list_node* node)
{
    node->next = previous->next;
    previous->next = node;
}

void dan_remove_list_node(dan_list_node* previous)
{
    previous->next = previous->next->next;
}

void dan_push_list_node(dan_list* list, dan_list_node* node)
{
    dan_insert_list_node(&(list->head),node);
}

void dan_pop_list_node(dan_list* list)
{
    dan_remove_list_node(&(list->head));
}

void dan_insert_list2_node(dan_list_node* previous, dan_list2_node* node)
{
    dan_insert_list_node(previous,&(node->node));
    node->previous = previous;
}

void dan_remove_list2_node(dan_list2_node* node)
{
    dan_remove_list_node(node->previous);
}

void dan_push_list2_node(dan_list2* list, dan_list2_node* node)
{
    dan_insert_list2_node(&(list->head),node);
}

void dan_pop_list2_node(dan_list2* list)
{
    dan_remove_list_node(&(list->head));
}

dan_list_node* dan_first_list_node(dan_list* list)
{
    return list->head.next;
}

dan_list2_node* dan_first_list2_node(dan_list2* list)
{
    return (dan_list2_node*) dan_first_list_node(list);
}

dan_list2_node* dan_next_list2_node(dan_list2_node* node)
{
    return (dan_list2_node*) node->node.next;
}

