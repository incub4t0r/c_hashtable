/**
 * @file errorcode.h
 * @author Daniel Chung
 * @brief Headerfile for program errorcodes.
 * @version 0.1
 * @date 2023-12-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ERRORCODE_H
#define ERRORCODE_H

enum error_t
{
    E_SUCCESS = 0,
    E_GENERAL,
    E_NULL_PTR,
    E_HASHTABLE_NOT_FOUND,
    E_HASHTABLE_CREATE,
    E_HASHTABLE_INSERT,
    E_HASHTABLE_DELETE,
    E_HASHTABLE_DESTROY,
    E_NODE_NOT_FOUND,
};

typedef enum error_t error_t;

struct error_desc_t
{
    error_t      code;
    const char * desc;
};

extern struct error_desc_t error_desc_t[];

#endif /*** ERRORCODE_H ***/

/*** end of file ***/