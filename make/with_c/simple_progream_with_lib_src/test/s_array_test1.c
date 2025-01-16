#include <stdio.h>
#include "cc_array.h"
#include <assert.h>

struct test_struct{
    int infoa;
    int infob;
    int infoc;
    float infod;
};

int main(void)
{
    struct cc_array *array;
    struct test_struct tmp;
    struct test_struct *tmpa;

    if(cc_array_new(&array, 10, sizeof(struct test_struct))) {
        printf("error \n");
    }

    for(int i = 0; i < 10; i++)
    {
        tmp.infoa = i;
        tmp.infob = i*2;
        tmp.infoc = i*3;
        tmp.infod = i*0.03;
        cc_array_set(array, i, &tmp);
    }

    for(int i = 0; i < 10; i++)
    {
        cc_array_get(array, i, &tmp);
        printf("i: %d %d %d %d %f\n",i,tmp.infoa,tmp.infob,tmp.infoc,tmp.infod);
    }

    if(cc_array_get_ref(array, 5, &tmpa))
    {
        printf("------err 2---------\n");
        return 0;
    }
    printf("!!!!!!!!%d %d %d\n",tmpa->infoa,tmpa->infob,tmpa->infoc);


    cc_array_delete(array);
    return 0;
}