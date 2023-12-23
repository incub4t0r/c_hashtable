/**
 * @file hashtable.c
 * @author Daniel Chung
 * @brief A hashtable implementation using separate chaining and murmurhash3.
 * @version 0.1
 * @date 2023-12-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/errorcode.h"

/**
 * @brief A global array full of prime numbers
 * The largest prime number can account for 4,412,637 years. This *should* be
 * enough.
 * Credit: https://planetmath.org/goodhashtableprimes
 */
uint32_t g_primes[]
    = { 53,        97,        193,      389,       769,       1543,
        3079,      6151,      12289,    24593,     49157,     98317,
        196613,    393241,    786433,   1572869,   3145739,   6291469,
        12582917,  25165843,  50331653, 100663319, 201326611, 402653189,
        805306457, 1610612741 };

uint32_t hash_str (char * p_key);
uint32_t murmurhash (const void * p_key, int len, uint32_t seed);
node_t * node_create (char * p_key, void * p_value);

/**
 * @brief Creates a new hashtable on heap.
 *
 * @param prime_index The index of the prime number to use for the hashtable
 * @return ht_t* On success, returns a pointer to the newly created hashtable,
 * else NULL.
 */
ht_t * ht_create (int prime_index)
{
    ht_t * new_ht = malloc(sizeof(ht_t));

    if (NULL == new_ht)
    {
        goto EXIT;
    }

    new_ht->size        = 0;
    new_ht->prime_index = prime_index;
    new_ht->capacity    = g_primes[new_ht->prime_index];
    new_ht->pp_items    = calloc(new_ht->capacity, sizeof(node_t *));

    if (NULL == new_ht->pp_items)
    {
        perror("Failed to calloc memory for hashtable items\n");
        free(new_ht);
        new_ht = NULL;
        goto EXIT;
    }

EXIT:
    return (new_ht);
}

/**
 * @brief Destroys a hashtable.
 *
 * @param p_ht Pointer to the hashtable to be destroyed.
 * @return error_t On success, returns 0, else non zero error
 */
error_t ht_destroy (ht_t * p_ht)
{
    error_t retval = E_GENERAL;

    if (NULL == p_ht)
    {
        retval = E_NULL_PTR;
        goto EXIT;
    }

    for (uint32_t cap_idx = 0; cap_idx < p_ht->capacity; cap_idx++)
    {
        node_t * current = p_ht->pp_items[cap_idx];

        while (current != NULL)
        {
            node_t * temp = current;
            current       = current->p_next;
            free(temp);
        }
    }

    free(p_ht->pp_items);
    free(p_ht);
    p_ht   = NULL;
    retval = E_SUCCESS;

EXIT:
    return (retval);
}

/**
 * @brief Inserts a new node into the hashtable.
 *
 * @param pp_ht A pointer to the pointer to the hashtable.
 * @param p_key Char pointer to the p_key of the new node.
 * @param p_value Void pointer to the value of the new node.
 * @return error_t On success, returns 0, else non zero error.
 */
error_t ht_insert (ht_t ** pp_ht, char * p_key, void * p_value)
{
    error_t retval = E_GENERAL;

    if (NULL == pp_ht || NULL == *pp_ht)
    {
        retval = E_NULL_PTR;
        goto EXIT;
    }

    node_t * p_new_node = node_create(p_key, p_value);

    if (NULL == p_new_node)
    {
        retval = E_NULL_PTR;
        goto EXIT;
    }

    uint32_t hash  = hash_str(p_key);
    uint32_t index = hash % (*pp_ht)->capacity;

    if (NULL == (*pp_ht)->pp_items[index])
    {
        (*pp_ht)->size++;
    }
    else
    {
        p_new_node->p_next = (*pp_ht)->pp_items[index];
    }

    (*pp_ht)->pp_items[index] = p_new_node;
    // typecast to float to avoid integer division
    float load_factor = (float)(*pp_ht)->size / (*pp_ht)->capacity;

    // if load factor is greater than 0.8, resize the hashtable
    if (load_factor > 0.8)
    {
        ht_t * p_new_ht = ht_create(((*pp_ht)->prime_index) + 1);

        for (uint32_t cap_idx = 0; cap_idx < (*pp_ht)->capacity; cap_idx++)
        {
            node_t * p_node = (*pp_ht)->pp_items[cap_idx];

            while (p_node != NULL)
            {
                retval = ht_insert(&p_new_ht, p_node->p_key, p_node->p_value);

                if (E_SUCCESS != retval)
                {
                    fprintf(stderr,
                            "Could not insert into new hashtable: %s\n",
                            error_desc_t[retval].desc);
                    goto EXIT;
                }

                p_node = p_node->p_next;
            }
        }

        retval = ht_destroy(*pp_ht);

        if (E_SUCCESS != retval)
        {
            fprintf(stderr,
                    "Could not destroy old hashtable: %s\n",
                    error_desc_t[retval].desc);
            goto EXIT;
        }

        *pp_ht = p_new_ht;
    }

    retval = E_SUCCESS;
EXIT:
    return (retval);
}

/**
 * @brief Deletes a node from the hashtable.
 *
 * @param p_ht Pointer to the hashtable
 * @param p_key Char pointer to the p_key of the node to be deleted.
 * @return error_t On success, returns 0, else non zero error.
 */
error_t ht_delete (ht_t * p_ht, char * p_key)
{
    error_t retval = E_GENERAL;

    if (NULL == p_ht)
    {
        retval = E_NULL_PTR;
        goto EXIT;
    }

    if (NULL == p_key)
    {
        retval = E_NULL_PTR;
        goto EXIT;
    }

    uint32_t  hash  = hash_str(p_key);
    uint32_t  index = hash % p_ht->capacity;
    node_t ** node  = &(p_ht->pp_items[index]);

    while (NULL != *node)
    {
        if (0 == strncmp(p_key, (*node)->p_key, strlen(p_key)))
        {
            printf("Found node to delete\n");
            node_t * to_delete = *node;
            *node              = (*node)->p_next;
            free(to_delete);
            p_ht->size--;
            retval = E_SUCCESS;
            goto EXIT;
        }

        node = &((*node)->p_next);
    }

    retval = E_NODE_NOT_FOUND;
EXIT:
    return (retval);
}

/**
 * @brief Searches the hashtable for a p_key.
 *
 * @param p_ht Pointer to the hashtable.
 * @param p_key Char pointer to the p_key to be searched for.
 * @return void* On success, returns a pointer to the value of the p_key, else
 * NULL.
 */
void * ht_search (ht_t * p_ht, char * p_key)
{
    void * retval = NULL;

    if (NULL == p_ht)
    {
        goto EXIT;
    }

    uint32_t hash  = hash_str(p_key);
    uint32_t index = hash % p_ht->capacity;
    node_t * node  = p_ht->pp_items[index];

    while (NULL != node)
    {
        if (0 == strcmp(p_key, node->p_key))
        {
            retval = node->p_value;
            goto EXIT;
        }

        node = node->p_next;
    }

EXIT:
    return (retval);
}

/**
 * @brief A private function to create a new node
 *
 * @param p_key Pointer to the p_key of the new node.
 * @param p_value Pointer to the value of the new node.
 * @return node_t* On success, returns a pointer to the newly created node, else
 * NULL;
 */
node_t * node_create (char * p_key, void * p_value)
{
    node_t * new_node = malloc(sizeof(node_t));

    if (new_node)
    {
        new_node->p_key   = p_key;
        new_node->p_value = p_value;
        new_node->p_next  = NULL;
    }

    return (new_node);
};

/**
 * @brief Hashes a string p_key.
 *
 * @param p_key Pointer to the string p_key to be hashed.
 * @return uint32_t Returns the hash value of the p_key.
 */
uint32_t hash_str (char * p_key)
{
    uint32_t hash = murmurhash(p_key, strlen(p_key), 0);
    return (hash);
}

// NOLINTBEGIN
/**
 * @brief An implementation of MurmurHash3.
 * Credit: https://github.com/jwerle/murmurhash.c
 *
 * @param p_key Pointer to the p_key to be hashed.
 * @param len Length of the p_key.
 * @param seed Seed value for the hash.
 * @return uint32_t Returns the hash value of the p_key.
 */

uint32_t murmurhash (const void * key_ptr, int length, uint32_t seed)
{
    uint32_t         const1     = 0xcc9e2d51;
    uint32_t         const2     = 0x1b873593;
    uint32_t         shift1     = 15;
    uint32_t         shift2     = 13;
    uint32_t         multiplier = 5;
    uint32_t         adder      = 0xe6546b64;
    uint32_t         hash       = 0;
    uint32_t         key_chunk  = 0;
    uint8_t *        data = (uint8_t *)key_ptr; // 32 bit extract from `p_key'
    const uint32_t * chunks_ptr = NULL;
    const uint8_t *  tail_ptr   = NULL; // tail - last 8 bytes
    int              index      = 0;
    int              chunk_len  = length / 4; // chunk length
    hash                        = seed;
    chunks_ptr                  = (const uint32_t *)(data + chunk_len * 4);
    tail_ptr                    = (const uint8_t *)(data + chunk_len * 4);

    // for each 4 byte chunk of `p_key'
    for (index = -chunk_len; index != 0; ++index)
    {
        // next 4 byte chunk of `p_key'
        key_chunk = chunks_ptr[index];
        // encode next 4 byte chunk of `p_key'
        key_chunk *= const1;
        key_chunk = (key_chunk << shift1) | (key_chunk >> (32 - shift1));
        key_chunk *= const2;
        // append to hash
        hash ^= key_chunk;
        hash = (hash << shift2) | (hash >> (32 - shift2));
        hash = hash * multiplier + adder;
    }

    key_chunk = 0;

// In this switch case, we want the cases to fall through.
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
    switch (length & 3)
    { // `len % 4'
        case 3:
            key_chunk ^= (tail_ptr[2] << 16);
        case 2:
            key_chunk ^= (tail_ptr[1] << 8);
        case 1:
            key_chunk ^= tail_ptr[0];
            key_chunk *= const1;
            key_chunk = (key_chunk << shift1) | (key_chunk >> (32 - shift1));
            key_chunk *= const2;
            hash ^= key_chunk;
    }

    hash ^= length;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

// NOLINTEND

/*** end of file ***/