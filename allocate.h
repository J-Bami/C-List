#ifndef ALLOCATE_H
#define ALLOCATE_H
#include <stddef.h>

#ifndef SIZEOF_PTR
    #define SIZEOF_PTR sizeof(void*)
#endif
typedef char byte;
void check_ptr(void *ptr);
void *allocate(const size_t size_bytes);
void *c_allocate(const size_t size_bytes, const size_t number);
void *reallocate(void *ptr, const size_t new_size_bytes);
void *copy_array(void *ptr, const size_t size_in_bytes);
#endif