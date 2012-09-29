
#include <stdlib.h>
#include <stdio.h>
#include "../dan_aa.h"

typedef struct
{
    dan_aa_node node;
    int key;
} int_node;

static void print_tree_r(dan_aa_tree t, int indent)
{
    if (t->left == t)
        return;
    print_tree_r(t->left,indent+1);
    for (int i=0; i < indent; ++i) putchar(' ');
    printf("%d\n",((int_node*)t)->key);
    print_tree_r(t->right,indent+1);
}

static void print_tree(dan_aa_tree t)
{
    print_tree_r(t,0);
    printf("\n");
}

bool int_less(dan_aa_node* a, dan_aa_node* b)
{
    return ((int_node*)a)->key < ((int_node*)b)->key;
}

static void insert_tree(int n, dan_aa_tree* t)
{
    int_node* x = malloc(sizeof(*x));
    x->key = n;
    dan_aa_node* result = dan_aa_insert((dan_aa_node*)x,t,int_less);
    if (result == (dan_aa_node*)x)
        printf("inserted %d\n",x->key);
    else
        printf("did not insert %d\n",x->key);
    print_tree(*t);
}

static void delete_tree(int n, dan_aa_tree* t)
{
    int_node x;
    x.key = n;
    dan_aa_node* result = dan_aa_remove((dan_aa_node*)&x,t,int_less);
    if (((int_node*)result)->key == n)
    {
        printf("deleted %d\n",n);
        free(result);
    }
    else
        printf("did not delete %d, %p\n",n,result);
    print_tree(*t);
}

int main(int argc, char** argv)
{
    dan_aa_tree tree = DAN_AA_TREE_INIT;
    insert_tree(0,&tree);
    insert_tree(1,&tree);
    insert_tree(-1,&tree);
    insert_tree(100,&tree);
    insert_tree(50,&tree);
    delete_tree(50,&tree);
    delete_tree(-1,&tree);
    delete_tree(100,&tree);
    delete_tree(1,&tree);
    delete_tree(0,&tree);
    return 0;
}
