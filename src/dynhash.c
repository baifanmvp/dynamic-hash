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
    DYNHNODE* hnode = 0;
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
    unsigned long hashval = phash->fnhash((unsigned char*)key);
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
    uchar * key = phash->getkey(data);
    unsigned long idx = dynhash_hash_pos(phash, key);

    DYNHNODE* hnode;
    DYNARRAY_IDX(phash->array, idx, hnode);
    
        
    char* lp_org_key = phash->getkey(hnode->data);

    //检测这个位置的值是真实值还是冲突的值
    if(dynhash_hash_pos(phash, lp_org_key) != idx)
    {
        return NULL;
    }
    
    size_t n_next_idx = idx;

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



    
    size_t n_old_idx = phash->hlen - (phash->hmod >> 1);
    size_t n_new_idx = phash->hlen;
    size_t n_empty_idx = phash->hlen;
    DYNHNODE* p_old_node;
    DYNHNODE* p_new_node = NULL;
    DYNHNODE* p_empty_node = NULL;
    
    if(phash->hlen != n_old_idx)
    {
        //扩增一个空node，hlen加大了1个，则空node会对应前头一个node里的key hash会根据这个值的增大而变化，重新算得的值一定会对应落在在新旧2个node里面 （看pdf的解释）
        phash->hlen += 1;
        
        DYNARRAY_IDX(phash->array, n_old_idx, p_old_node);
        
        char* lp_old_key = phash->getkey(p_old_node->data);
        unsigned long n_real_idx = dynhash_hash_pos(phash, lp_old_key);

        if(n_real_idx == n_old_idx || n_real_idx == n_new_idx)
        {
            
            size_t n_last_idx = n_old_idx;
            size_t n_tail_idx = n_new_idx;
            size_t n_iter_idx = n_old_idx;
            DYNHNODE* p_iter_node;
            DYNHNODE* p_last_node;
            DYNHNODE* p_tail_node;
            
            while(n_iter_idx != HNODE_INVALID_IDX)
            {
                DYNARRAY_IDX(phash->array, n_iter_idx, p_iter_node);
                
                char* lp_old_key = phash->getkey(p_iter_node->data);
                n_real_idx = dynhash_hash_pos(phash, lp_old_key);

                if(n_real_idx == n_new_idx)
                {
                    if((size_t)n_iter_idx != (size_t)n_last_idx)
                    {
                        DYNARRAY_IDX(phash->array, n_last_idx, p_last_node);
                        p_last_node->next = p_iter_node->next;
                        n_iter_idx = p_iter_node->next;
                    }
                    else
                    {
                        //delete list head
                        n_old_idx = p_iter_node->next;
                        p_old_node = p_iter_node;
                        n_last_idx = p_iter_node->next;
                        n_iter_idx = p_iter_node->next;
                    }
                    ////////////////////////////////
                    if(p_new_node)
                    {
                        p_iter_node->next = HNODE_INVALID_IDX;
                        p_tail_node->next = n_iter_idx;
                        p_tail_node = p_iter_node;
                    }
                    else
                    {
                        p_iter_node->next = HNODE_INVALID_IDX;
                        p_new_node =  p_iter_node;
                        p_tail_node = p_iter_node;
                    }
                }
                else
                {
                    n_last_idx = n_iter_idx;
                    n_iter_idx = p_iter_node->next;
                }
                
            }
            //move new_node to empty_node
            DYNARRAY_IDX(phash->array, n_empty_idx, p_empty_node);
            p_empty_node->data = p_new_node->data;
            p_empty_node->next = p_new_node->next;
            p_empty_node = p_new_node;

            //看看老的list头是否改变过，改变了说明老的list的值算出的位置是属于new node，且已经在上两行代码中已经移动过去了
            if( n_old_idx != ((phash->hlen - 1) - (phash->hmod >> 1)) )
            {
                //move old_node head to origin position
                p_empty_node->data = p_old_node->data;
                p_empty_node->next = p_old_node->next;
                p_empty_node = p_old_node;
            }
        }
        else
        {
            //这个位置被其他hash链的node所占据，也说明前面加入的node的hash值没有一个是真正属于这个位置的,所以什么也不要做
        }


        //下面是插入新的node
        char* lp_insert_key = phash->getkey(data);
        unsigned long n_insert_idx = dynhash_hash_pos(phash, lp_insert_key);
        DYNHNODE* p_insert_node;
        DYNARRAY_IDX(phash->array, n_insert_idx, p_insert_node);
        
        if(p_empty_node == p_insert_node)
        {
            p_empty_node->data = data;
            p_empty_node->next = HNODE_INVALID_IDX;
        }
        else
        {
            unsigned long n_real_idx = dynhash_hash_pos(phash, p_insert_node->data);
            size_t n_iter_idx = n_real_idx;
            DYNHNODE* p_iter_node;

            if(n_real_idx == n_insert_idx)
            {
                while(n_iter_idx != HNODE_INVALID_IDX)
                {
                    DYNARRAY_IDX(phash->array, n_iter_idx, p_iter_node);
                    n_iter_idx = p_iter_node->next;
                }
                p_iter_node->next = DYNARRAY_GET_IDX(phash->array, p_empty_node);
                
                p_empty_node->data = data;
                p_empty_node->next = HNODE_INVALID_IDX;
                
            }
            else
            {
                while(n_iter_idx != HNODE_INVALID_IDX)
                {
                    DYNARRAY_IDX(phash->array, n_iter_idx, p_iter_node);
                    if(p_iter_node->next == n_insert_idx)
                    {
                        //find last node of insert node
                        break;
                    }
                    
                    n_iter_idx = p_iter_node->next;
                }
                p_iter_node->next = DYNARRAY_GET_IDX(phash->array, p_empty_node);
                p_empty_node->data = p_insert_node->data;
                p_empty_node->next = p_insert_node->next;
                p_insert_node->data = data;
                p_empty_node->next = HNODE_INVALID_IDX;
                
            }
            
        }
        
    }
    else
    {
        DYNHNODE* p_insert_node;
        DYNARRAY_IDX(phash->array, phash->hlen, p_insert_node);
        p_insert_node->data = data;
        p_insert_node->next = HNODE_INVALID_IDX;
        
    }

}
