#ifndef DYNARRAY_H_
#define DYNARRAY_H_
typedef struct st_dynarray {
    // total cnt
    uint32_t acnt;
    // use cnt
    uint32_t ucnt;
    // grow cnt
    uint32_t gcnt;
    
    // total cnt
    uint16_t unitsz;
} DYNARRAY;

#define DYNARRAY_INIT(array, gcnt, unitsz)                  \
    do                                                      \
    {                                                       \
        array = malloc(sizeof(DYNARRAY)+(gcnt)*(unitsz));   \
        array->acnt = gcnt;                                 \
        array->ucnt = 0;                                    \
        array->gcnt = gcnt;                                 \
        array->unitsz = unitsz;                             \
    }                                                       \
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
        if (idx < (array)->ucnt)                                \
        {                                                       \
            ptr = (char*)((array) + 1) + (array)->unitsz * idx; \
        }                                                       \
        else                                                    \
        {                                                       \
            ptr = 0;                                            \
        }                                                       \
    }                                                           \
    while(0);


#endif
