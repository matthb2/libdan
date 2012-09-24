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

#ifndef DAN_AA_H
#define DAN_AA_H

#include <stdbool.h>

typedef struct dan_aa_node_struct* dan_aa_tree;

struct dan_aa_node_struct
{
    dan_aa_tree left;
    dan_aa_tree right;
    int level;
};
typedef struct dan_aa_node_struct dan_aa_node;

typedef bool (*dan_aa_less)(dan_aa_node* a, dan_aa_node* b);

void dan_aa_tree_init(dan_aa_tree* t);
dan_aa_node* dan_aa_insert(dan_aa_node* x, dan_aa_tree* t, dan_aa_less less);
dan_aa_node* dan_aa_delete(dan_aa_node* x, dan_aa_tree* t, dan_aa_less less);

#endif
