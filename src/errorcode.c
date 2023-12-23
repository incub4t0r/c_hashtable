/**
 * @file errorcode.c
 * @author Daniel Chung
 * @brief Error code descriptions
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/errorcode.h"

struct error_desc_t error_desc_t[] = {
    { E_SUCCESS, "Success, no error" },
    { E_GENERAL, "General error" },
    { E_NULL_PTR, "Null pointer" },
    { E_HASHTABLE_NOT_FOUND, "Hashtable not found" },
    { E_HASHTABLE_CREATE, "Hashtable creation error" },
    { E_HASHTABLE_INSERT, "Hashtable insertion error" },
    { E_HASHTABLE_DELETE, "Hashtable deletion error" },
    { E_HASHTABLE_DESTROY, "Hashtable destruction error" },
    { E_NODE_NOT_FOUND, "Node not found" },
};

/*** end of file ***/