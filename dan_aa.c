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

/*
   These functions and data structures are from:
      A. Andersson, “Balanced Search Trees Made Simple,” pp. 60-71, 1993.
   The "aa" prefix is for Arne Andersson.
   He deserves full credit for their functionality and simplicity.
   I have modified them such that nodes are intrusive rather than
   containing a key which is the reason for the "less" function pointers
   and the more complicated remove function.
*/

#include <stdlib.h>
#include <stdio.h>
#include "dan_aa.h"

dan_aa_node dan_aa_bottom = DAN_AA_NODE_INIT;

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
    if (*t == &dan_aa_bottom)
    {
        result = x;
        *t = x;
        (*t)->left = &dan_aa_bottom;
        (*t)->right = &dan_aa_bottom;
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

struct remove_vars
{
    dan_aa_node* x;
    dan_aa_less less;
    dan_aa_node* deleted;
    dan_aa_node* last;
    dan_aa_node* successor;
};

static void remove_successor(dan_aa_tree* t, struct remove_vars* v)
{
    if (*t != &dan_aa_bottom)
    { /* search down the tree and set pointers last and deleted */
        v->last = *t;
        if (v->less(v->x,*t))
            remove_successor(&((*t)->left),v);
        else
        {
            v->deleted = *t;
            remove_successor(&((*t)->right),v);
        }
    }
    /* at the bottom of the tree we remove the element (if it is present) */
    if ((*t == v->last)&&(v->deleted != &dan_aa_bottom)
        &&(!v->less(v->x,v->deleted))&&(!v->less(v->deleted,v->x)))
    {
        v->successor = *t;
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
}

static void swap_successor(dan_aa_tree* t, struct remove_vars* v)
{
    if (v->less(v->deleted,*t))
        swap_successor(&((*t)->left),v);
    else if (v->less(*t,v->deleted))
        swap_successor(&((*t)->right),v);
    else
    {
        if (v->deleted != *t)
            abort();
        *t = v->successor;
        (*t)->left = v->deleted->left;
        (*t)->right = v->deleted->right;
        (*t)->level = v->deleted->level;
    }
}

dan_aa_node* dan_aa_remove(dan_aa_node* x, dan_aa_tree* t, dan_aa_less less)
{
    struct remove_vars v;
    v.x = x;
    v.less = less;
    v.deleted = &dan_aa_bottom;
    v.last = 0;
    v.successor = 0;
    remove_successor(t,&v);
    if (!v.successor)
        return 0;
    if (v.successor != v.deleted)
        swap_successor(t,&v);
    return v.deleted;
}

dan_aa_node* dan_aa_find(dan_aa_node* x, dan_aa_tree t, dan_aa_less less)
{
    if (t == &dan_aa_bottom)
        return 0;
    if (less(x,t))
        return dan_aa_find(x,t->left,less);
    else if (less(t,x))
        return dan_aa_find(x,t->right,less);
    else
        return t;
}

