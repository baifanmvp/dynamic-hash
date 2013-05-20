#include "dynhash.h"

#include <stdint.h>
#include <stdio.h>

uchar* getkey (const uchar * key)
{
    return key;
}

void freekey (void * key)
{
    free(key);
}


int main()
{

    DYNHASH* phash = dynhash_new(getkey, freekey, hashatoi);
    int start = 3;
    int i = 0;
    DYNARRAY* array = NULL;
    DYNARRAY_INIT(array, 4096, sizeof(int));
    while(i++ < 10)
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
