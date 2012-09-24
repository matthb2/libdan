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
{ .left = &bottom_node, .right = &bottom_node, .level = 0, .key = 0 };
static dan_aa_tree bottom = &bottom_node;
static dan_aa_tree deleted = &bottom_node;
static dan_aa_tree last;

static void dan_aa_new(dan_aa_tree* t)
{
    *t = malloc(sizeof(**t));
}

static void dan_aa_dispose(dan_aa_tree t)
{
    free(t);
}

void dan_aa_init()
{
}

void dan_aa_tree_init(dan_aa_tree* t)
{
    *t = bottom;
}

static void dan_aa_skew(dan_aa_tree* t)
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

static void dan_aa_split(dan_aa_tree* t)
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

void dan_aa_insert(int x, dan_aa_tree* t, bool* ok)
{
    if (*t == bottom)
    {
        dan_aa_new(t);
        (*t)->key = x;
        (*t)->left = bottom;
        (*t)->right = bottom;
        (*t)->level = 1;
        *ok = true;
    }
    else
    {
        if (x < (*t)->key)
            dan_aa_insert(x,&((*t)->left),ok);
        else if (x > (*t)->key)
            dan_aa_insert(x,&((*t)->right),ok);
        else
            *ok = false;
        dan_aa_skew(t);
        dan_aa_split(t);
    }
}

void dan_aa_delete(int x, dan_aa_tree* t, bool* ok)
{
    *ok = false;
    if (*t != bottom)
    { /* search down the tree and set pointers last and deleted */
        last = *t;
        if (x < (*t)->key)
            dan_aa_delete(x,&((*t)->left),ok);
        else
        {
            deleted = *t;
            dan_aa_delete(x,&((*t)->right),ok);
        }
    }
    /* at the bottom of the tree we remove the element (if it is present) */
    if ((*t == last)&&(deleted != bottom)
        &&(x == deleted->key))
    {
        deleted->key = (*t)->key;
        deleted = bottom;
        *t = (*t)->right;
        dan_aa_dispose(last);
        *ok = true;
    }
    /* on the way back, we rebalance */
    else if (((*t)->left->level < (*t)->level-1)
             ||((*t)->right->level < (*t)->level-1))
    {
        --((*t)->level);
        if ((*t)->right->level > (*t)->level)
            (*t)->right->level = (*t)->level;
        dan_aa_skew(t);
        dan_aa_skew(&((*t)->right));
        dan_aa_skew(&((*t)->right->right));
        dan_aa_split(t);
        dan_aa_split(&((*t)->right));
    }
}

