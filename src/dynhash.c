#include "dynhash.h"
DYNHASH* dynhash_new(dynhash_get_key getkey, dynhash_free_key freekey, dynhash_function fnhash)
{
    DYNHASH* phash = (DYNHASH*)malloc(sizeof(DYNHASH));
    phash->hmod = 1;
    phash->hlen = 0;
    DYNARRAY_INIT(phash->array, 1024*8, sizeof(DYNHNODE));
    phash->getkey = getkey;
    phash->freekey = freekey;
    phash->fnhash = fnhash;
    return phash; 
}

int dynhash_delete(DYNHASH* phash)
{
    if(phash->hlen !=  phash->array->ucnt)
    {
        printf("[ERROR] phash->hlen !=  phash->array->ucnt! \n ");
        return 0;
    }
    DYNHNODE hnode = 0;
    while(phash->hlen--)
    {
        DYNARRAY_IDX(phash->array, phash->hlen, hnode);
        phash->freekey(hnode->data);
           
    }
    
    DYNARRAY_DESTROY(phash->array);
    free(phash);
    return 1; 
}



unsigned long dynhash_hash_pos(DYNHASH* phash, void* key)
{
    unsigned long hashval = phash->hashfun(key);
    if(hashval % phash->hmod < phash->hlen)
    {
        return hashval % phash->hmod;
    }
    else
    {
        return hashval % (phash->hmod >> 1);
    }
    
}



DYNHNODE* dynhash_search(DYNHASH* phash, void* data)
{
    if(!phash)
    {
        return NULL;
    }
    char* key = phash->getkey(data);
    unsigned long idx = dynhash_hash_pos(phash, key);

    DYNHNODE hnode;
    size_t n_next_idx = 0;
    DYNARRAY_IDX(phash->array, idx, hnode);
    
        
    char* lp_org_key = phash->getkey(hnode->data);

    //检测这个位置的值是真实值还是冲突的值
    if(dynhash_hash_pos(phash, lp_org_key) != idx)
    {
        return NULL;
    }

    do
    {
        DYNARRAY_IDX(phash->array, n_next_idx, hnode);
        lp_org_key = phash->getkey(hnode->data);
        if(strcmp(key, lp_org_key) == 0)
        {
            //find it!
            return hnode;
        }
        
    }while((n_next_idx = hnode->next) != 0);
    return NULL;
}


int dynhash_insert (DYNHASH* phash, void* data)
{
    if(!phash || !data)
    {
        return 0;
    }

    if(dynhash_search(phash, data))
    {
        return 0;
    }




    size_t n_check_idx = phash->hlen - (phash->hmod >> 1);
    size_t n_move_idx = phash->hlen;
    size_t n_empty_idx = phash->hlen;

    if(phash->hlen != n_check_idx)
    {
        
        DYNHNODE* p_check_node;
        DYNHNODE* p_move_node;
    
        DYNARRAY_IDX(phash->array, n_check_idx, p_check_node);
        DYNARRAY_IDX(phash->array, n_move_idx, p_move_node);
        
        char* lp_org_key = phash->getkey(p_check_node->data);
        unsigned long n_real_idx = dynhash_hash_pos(phash, lp_org_key);

        if(n_real_idx == n_check_idx || n_real_idx == n_move_idx)
        {
            
            size_t n_last_idx = n_check_idx;
            size_t n_tail_idx = n_move_idx;
            size_t n_iter_idx = n_move_idx;
            DYNHNODE* p_iter_node;
            DYNHNODE* p_last_node;
            DYNHNODE* p_tail_node;
            while(n_iter_idx)
            {
                DYNARRAY_IDX(phash->array, n_iter_idx, p_iter_node);
                
                char* lp_org_key = phash->getkey(p_iter_node->data);
                n_real_idx = dynhash_hash_pos(phash, lp_org_key);
                if(n_real_idx == n_check_idx)
                {
                    if(p_check_node->data == NULL)
                    {
                        p_check_node->data = p_iter_node->data;
                        p_check_node->next = p_iter_node->next;
                        n_empty_idx = n_iter_idx;
                    }
                }
                else if(n_real_idx == n_move_idx)
                {
                    if(p_move_node->data == NULL)
                    {
                        p_move_node->data = p_iter_node->data;
                        p_move_node->next = HNODE_INVALID_IDX;
                        n_empty_idx = n_iter_idx;
                        n_tail_idx = n_move_idx;
                        
                        DYNARRAY_IDX(phash->array, n_last_idx, p_last_node);
                        p_last_node->next = p_iter_node->next;
                    }
                    else
                    {
                        DYNARRAY_IDX(phash->array, n_tail_idx, p_tail_node);
                        DYNARRAY_IDX(phash->array, n_last_idx, p_last_node);
                        p_tail_node->next = n_iter_idx;
                        p_last_node->next = p_iter_node->next;
                        p_iter_node->next = HNODE_INVALID_IDX;
                    }
                }
                n_last_idx = n_iter_idx;
            }
        }
        else
        {
            
        }
    }
    else
    {
        
    }

        
    char* key = phash->getkey(data);
    unsigned long idx = dynhash_hash_pos(phash, key);

    DYNHNODE hnode;
    
    DYNARRAY_IDX(phash->array, idx, hnode);
    
        
    char* lp_org_key = phash->getkey(hnode->data);
    
    if(dynhash_hash_pos(phash, lp_org_key) == idx)
    {
        return NULL;
    }
    
}
