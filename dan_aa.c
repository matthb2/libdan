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

/* These functions and data structures are from:
      A. Andersson, “Balanced Search Trees Made Simple,” pp. 60-71, 1993.
   The "aa" prefix is for Arne Andersson.
   He deserves full credit for their functionality and simplicity.
*/

#include <stdlib.h>
#include <stdio.h>
#include "dan_aa.h"

static dan_aa_node bottom_node = 
{ .left = &bottom_node, .right = &bottom_node, .level = 0 };
static dan_aa_tree deleted = &bottom_node;
static dan_aa_tree last;

void dan_aa_tree_init(dan_aa_tree* t)
{
    *t = &bottom_node;
}

static void skew(dan_aa_tree* t)
{
    dan_aa_tree temp;
    if ((*t)->left->level == (*t)->level)
    { /* rotate right */
        temp = *t;
        *t = (*t)->left;
        temp->left = (*t)->right;
        (*t)->right = temp;
    }
}

static void split(dan_aa_tree* t)
{
    dan_aa_tree temp;
    if ((*t)->right->right->level == (*t)->level)
    { /* rotate left */
        temp = *t;
        *t = (*t)->right;
        temp->right = (*t)->left;
        (*t)->left = temp;
        ++((*t)->level);
    }
}

dan_aa_node* dan_aa_insert(dan_aa_node* x, dan_aa_tree* t, dan_aa_less less)
{
    dan_aa_node* result;
    if (*t == &bottom_node)
    {
        result = x;
        *t = x;
        (*t)->left = &bottom_node;
        (*t)->right = &bottom_node;
        (*t)->level = 1;
    }
    else
    {
        if (less(x,*t))
            result = dan_aa_insert(x,&((*t)->left),less);
        else if (less(*t,x))
            result = dan_aa_insert(x,&((*t)->right),less);
        else
            result = *t;
        skew(t);
        split(t);
    }
    return result;
}

static dan_aa_node* remove_successor(dan_aa_node* x, dan_aa_tree* t, dan_aa_less less)
{
    dan_aa_node* result = 0;
    if (*t != &bottom_node)
    { /* search down the tree and set pointers last and deleted */
        last = *t;
        if (less(x,*t))
            result = remove_successor(x,&((*t)->left),less);
        else
        {
            deleted = *t;
            result = remove_successor(x,&((*t)->right),less);
        }
    }
    /* at the bottom of the tree we remove the element (if it is present) */
    if ((*t == last)&&(deleted != &bottom_node)
        &&(!less(x,deleted))&&(!less(deleted,x)))
    {
        result = *t;
        *t = (*t)->right;
    }
    /* on the way back, we rebalance */
    else if (((*t)->left->level < (*t)->level-1)
             ||((*t)->right->level < (*t)->level-1))
    {
        --((*t)->level);
        if ((*t)->right->level > (*t)->level)
            (*t)->right->level = (*t)->level;
        skew(t);
        skew(&((*t)->right));
        skew(&((*t)->right->right));
        split(t);
        split(&((*t)->right));
    }
    return result;
}

static void swap_successor(dan_aa_node* successor, dan_aa_tree* t, dan_aa_less less)
{
    if (less(deleted,*t))
        swap_successor(successor,&((*t)->left),less);
    else if (less(*t,deleted))
        swap_successor(successor,&((*t)->right),less);
    else
    {
        if (deleted != *t)
            abort();
        *t = successor;
        (*t)->left = deleted->left;
        (*t)->right = deleted->right;
        (*t)->level = deleted->level;
    }
}

dan_aa_node* dan_aa_delete(dan_aa_node* x, dan_aa_tree* t, dan_aa_less less)
{
    dan_aa_node* result = remove_successor(x,t,less);
    if (!result)
        return result;
    if (result != deleted)
        swap_successor(result,t,less);
    result = deleted;
    deleted = &bottom_node;
    return result;
}

