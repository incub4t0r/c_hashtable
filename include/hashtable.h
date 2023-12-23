/**
 * @file hashtable.h
 * @author Daniel Chung
 * @brief Header file for the hashtable module.
 * @version 0.1
 * @date 2023-12-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>
#include <sys/types.h>
#include "errorcode.h"

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct node_t
{
    char *          p_key;
    void *          p_value;
    struct node_t * p_next;
} node_t;

typedef struct ht_t
{
    node_t ** pp_items;
    size_t    capacity;
    size_t    size;
    uint32_t  prime_index;
} ht_t;

ht_t *  ht_create (int prime_index);
error_t ht_destroy (ht_t * p_ht);
error_t ht_insert (ht_t ** pp_ht, char * p_key, void * p_value);
error_t ht_delete (ht_t * p_ht, char * p_key);
void *  ht_search (ht_t * p_ht, char * p_key);

#endif // HASHTABLE_H

/*** end of hashtable.h ***/
