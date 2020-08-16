
#include <stdlib.h>
#include <stdio.h>
#include "allocate.h"

void check_ptr(void *ptr)
{
    if (ptr == NULL)
    {
        printf("Could not allocate required memory. Exiting.\n");
        exit(1);
    }
}


void *allocate(const size_t size_bytes)
{
    void *out = NULL;
    if (size_bytes)
    {
        out = malloc(size_bytes);
        check_ptr(out);
    }
    return out;   
}

void *c_allocate(const size_t size_bytes, const size_t number)
{
    void *out = calloc(number, size_bytes);
    check_ptr(out);
    return out;   
}

void *reallocate(void *ptr, const size_t new_size_bytes)
{
    void *out = realloc(ptr, new_size_bytes);
    check_ptr(out);
    return out;
}

//copies the contents of the array to another
void *copy_array(void *ptr, const size_t size_in_bytes)
{
    byte *out = allocate(size_in_bytes);
    for (size_t i = 0; i < size_in_bytes; i++)
    {
        out[i] = ((byte *)ptr)[i];
    }
    return (void *)out;
}