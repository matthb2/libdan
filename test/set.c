
#include <stdio.h>
#include "../dan_set.h"

void print_set(dan_set* s)
{
    for (size_t i=0; i < s->count; ++i)
    {
        putchar(*((char*)s->objects[i]));
        putchar(' ');
    }
    putchar('\n');
}

int main(int argc, char** argv)
{
    char objects[4] = { 'A', 'B', 'C', 'D' };
    dan_set* s1 = dan_make_set(2);
    s1->objects[0] = objects+0;
    s1->objects[1] = objects+1;
    printf("set 1\n");
    print_set(s1);
    dan_set* s2 = dan_make_set(2);
    s2->objects[0] = objects+1;
    s2->objects[1] = objects+2;
    printf("set 2\n");
    print_set(s2);
    dan_set* s1_and_s2 = dan_set_intersection(s1,s2);
    printf("set 1 and 2\n");
    print_set(s1_and_s2);
    dan_set_free(s1);
    dan_set_free(s2);
    dan_set_free(s1_and_s2);
    return 0;
}
