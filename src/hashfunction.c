#include "hashfunction.h"
#include <stdlib.h>
unsigned long hashfun(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


unsigned long hashatoi(unsigned char *str)
{
    return atoi(str);
}
