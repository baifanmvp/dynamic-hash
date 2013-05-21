#include "dynhash.h"

#include <glib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

    
uchar* getkey (const uchar * key)
{
    return key;
}

void freekey (void * key)
{
    free(key);
}


int test1()
{

    DYNHASH* phash = dynhash_new(getkey, freekey, hashatoi);
    int start = 3;
    int i = 0;
    DYNARRAY* array = NULL;
    DYNARRAY_INIT(array, 4096, sizeof(int));
    srand(time(0));
    while(i++ < 10000)
    {
        printf("=========================%d====================\n", i);
        int random = rand()%10000;
        DYNARRAY_PUSH(array, &random)

        
        char* key = (char*)malloc(10);
        sprintf(key, "%d", random);
        printf("key [%s]\n", key);
        dynhash_insert(phash, key);

        int j = 0;
        while(j < i)
        {
            int* randtmp;
            DYNARRAY_IDX(array, j, randtmp);
                
            char tmp[10];
            sprintf(tmp, "%d", *randtmp);
            if(!dynhash_search(phash, tmp))
            {
                printf("key [%s] not find ^^^^^^^^^^^^^^^^^^^^^^^!\n", tmp);
            }
            j++;
        }


    }
    
    dynhash_delete(phash);




    
    return 0;
}

int testglib(int argc, char* argv[])
{  
    if(argc != 2)
    {
        printf("参数错误! \n");
        return -1;
    }

    int i = 0;
    int cnt = atoi(argv[1]);
    DYNARRAY* array = NULL;
    DYNARRAY_INIT(array, 4096, sizeof(int));
    srand(time(0));
    while(i++ < cnt)
    {
//        printf("=========================%d====================\n", i);
        int random = rand()%100000000;
        DYNARRAY_PUSH(array, &random)
    }


    struct timeval tv1;
    struct timeval tv2;
    ////////////////////////////glib hash//////////////////////////
    /* gettimeofday(&tv1, 0); */
    /* i = 0; */
        
    /* GHashTable * ghash = g_hash_table_new (g_str_hash, g_str_equal); */
    /* while(i < cnt) */
    /* { */
    /*     int* randtmp; */
    /*     DYNARRAY_IDX(array, i, randtmp); */

    /*     char* key = (char*)malloc(10); */
        
    /*     sprintf(key, "%d", *randtmp); */
    /*     //    printf("key [%s]\n", key); */
    /*     g_hash_table_insert (ghash, key, 0); */
    /*     i++; */
    /* } */
    /* printf("ghash insert ok!\n"); */
    /* sleep(100); */

    /* g_hash_table_destroy (ghash); */
    
    /* gettimeofday(&tv2, 0); */
    /* printf("ghash insert   time (%d data) : %f\n", i, (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec)/1000000 ); */


    ////////////////////////////dyn hash//////////////////////////
    gettimeofday(&tv1, 0);
        
    DYNHASH* dynhash = dynhash_new (getkey, freekey, hashfun);
    i = 0;
    while(i < cnt)
    {
        int* randtmp;
        DYNARRAY_IDX(array, i, randtmp);

        char* key = (char*)malloc(10);
        
        sprintf(key, "%d", *randtmp);
        //    printf("key [%s]\n", key);
        dynhash_insert (dynhash, key);
        i++;
    }
    printf("dynhash insert ok!\n");
//    sleep(100);

    dynhash_delete(dynhash);
    
    gettimeofday(&tv2, 0);
    printf("dynhash insert time (%d data) : %f\n", i, (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec)/1000000 );

    return 0;
}



int main(int argc, char* argv[])
{
    test1();
//    testglib(argc, argv);
}
