#ifndef DYNARRAY_H_
#define DYNARRAY_H_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct st_dynarray
{
    // total cnt
    uint32_t acnt;
    // use cnt
    uint32_t ucnt;
    // grow cnt
    uint32_t gcnt;
    
    // total cnt
    uint16_t unitsz;
} DYNARRAY;

#define DYNARRAY_INIT(ar, gt, usz)                              \
    do                                                          \
    {                                                           \
        ar = (DYNARRAY*)malloc(sizeof(DYNARRAY)+(gt)*(usz));    \
        ar->acnt = gt;                                          \
        ar->ucnt = 0;                                           \
        ar->gcnt = gt;                                          \
        ar->unitsz = usz;                                       \
    }                                                           \
    while(0);


#define DYNARRAY_DESTROY(array)                 \
    do                                          \
    {                                           \
        free(array);                            \
    }                                           \
    while(0);


#define DYNARRAY_PUSH(array, data)                                      \
    do                                                                  \
    {                                                                   \
        if(array->acnt == array->ucnt)                                  \
        {                                                               \
            array->acnt += array->gcnt;                                 \
            array = realloc(array, sizeof(DYNARRAY) + (array)->acnt * (array)->unitsz ); \
        }                                                               \
        memcpy((char*)((array) + 1) + (array)->unitsz * array->ucnt, data, (array)->unitsz); \
        array->ucnt++;                                                  \
    }                                                                   \
    while(0);


#define DYNARRAY_IDX(array, idx, ptr)                           \
    do                                                          \
    {                                                           \
        if ((uint32_t)(idx) < (array)->ucnt)                            \
        {                                                       \
            ptr = (char*)((array) + 1) + (array)->unitsz * idx; \
        }                                                       \
        else                                                    \
        {                                                       \
            ptr = 0;                                            \
        }                                                       \
    }                                                           \
    while(0);

#define DYNARRAY_GET_IDX(array, ptr)                                    \
    (((size_t)(ptr) - (size_t)(array) - sizeof(DYNARRAY))/(array)->unitsz)

#endif
