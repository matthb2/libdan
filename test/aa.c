
#include <stdio.h>
#include "../dan_aa.h"

static void print_tree_r(dan_aa_tree t, int indent)
{
    if (t->left == t)
        return;
    print_tree_r(t->left,indent+1);
    for (int i=0; i < indent; ++i) putchar(' ');
    printf("%d\n",t->key);
    print_tree_r(t->right,indent+1);
}

static void print_tree(dan_aa_tree t)
{
    print_tree_r(t,0);
    printf("\n");
}

static void insert_tree(int x, dan_aa_tree* t)
{
    bool ok;
    dan_aa_insert(x,t,&ok);
    if (ok)
        printf("inserted %d\n",x);
    else
        printf("did not insert %d\n",x);
    print_tree(*t);
}

static void delete_tree(int x, dan_aa_tree* t)
{
    bool ok;
    dan_aa_delete(x,t,&ok);
    if (ok)
        printf("deleted %d\n",x);
    else
        printf("did not delete %d\n",x);
    print_tree(*t);
}

int main(int argc, char** argv)
{
    dan_aa_init();
    dan_aa_tree tree;
    dan_aa_tree_init(&tree);
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
