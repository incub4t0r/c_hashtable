#include <stdio.h>
#include <stdlib.h>
#include "../include/hashtable.h"
#include "../include/errorcode.h"

int main (void)
{
    ht_t * p_ht = ht_create(0);

    if (NULL == p_ht)
    {
        printf("Failed to create hashtable\n");
        return (EXIT_FAILURE);
    }

    ht_destroy(p_ht);

    return (EXIT_SUCCESS);
}