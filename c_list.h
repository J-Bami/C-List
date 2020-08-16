/*
A module to define a bunch of macros for an object similar to a std::vector in C++
we will structure the list as so:
[length (size_t)][capacity (size_t)]*ptr[data item 1, data item 2,...]
macro functions include:

- declaration?  <not implemented>. probably not needed
- initialise    (internal), tested
- get_type_size (internal), tested
- get_length    (internal), tested
- get_capacity  (internal), tested
- grow          (internal), tested
- shrink        (internal), tested
- delete        tested
- append        tested
- insert        tested
- pop           tested
- push          tested
- remove        tested

The main advantage of using a data structure like this is that it will work for any array
type, with only one level of indirection.
*/

#ifndef CLIST_H
#define CLIST_H

#include "allocate.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define GROWTH_FACTOR 1.5
#define SHRINK_FACTOR 0.5
#define DEFAULT_CAPACITY ((size_t) 3)
#define MIN_CAPACITY ((size_t) 1)
#define incrementor ((size_t) 1)

#ifndef SIZEOF_ULONG
    #define SIZEOF_ULONG (sizeof(size_t))
#endif


//Sets the array up as a clist. the identifier must be an array set to NULL.
#define clist_initialise(identifier, typename)\
do{\
    if(!(identifier)){\
        size_t data_size = sizeof(typename);\
        size_t *temp = allocate((2*SIZEOF_ULONG) + (DEFAULT_CAPACITY*data_size));\
        (identifier) = ((void*) (&temp[2]));\
        (temp)[0] = 0;\
        (temp)[1] = DEFAULT_CAPACITY;\
    }\
}while(0);

//getter functions


//retrieves the address that the dynamic memory starts from 
#define clist_start(clist) ((clist)? (void*) (&(((size_t*)(clist))[-2])) : (size_t)0)


//gets the size of the datatype
#define clist_datasize(clist) ((clist)?(size_t) (sizeof(*(clist))) : (size_t)0) // change to get size of first item


//gets the length of the list
#define clist_length(clist) ((clist)?((size_t *)(clist))[-2] : (size_t)0)


// gets the capacity of the list
#define clist_capacity(clist) ((clist)?((size_t *)(clist))[-1] : (size_t)0)


// frees memory associated with the clist
#define clist_delete(clist) (free(clist_start(clist)))

//setter functions:


// changes the value in clist[-2]
#define __clist_set_length(clist, length)\
do{\
    if(clist)\
    {\
        ((size_t *)(clist))[-2] = (size_t) (length);\
    }\
}while(0);


// changes the value in clist[-1]
#define __clist_set_capacity(clist, capacity)\
do{\
    if(clist)\
    {\
        ((size_t *)(clist))[-1] = (size_t) (capacity);\
    }\
}while(0);


//other IO


// print useful stats for the list
#define print_clist(clist)\
do{\
    printf("clist: datatype_size: %lu, length: %lu, capacity: %lu @ 0x%x\n",\
    clist_datasize(clist), clist_length(clist), clist_capacity(clist), clist_start(clist));\
}while(0);


// changing the array capacity.
#define clist_change_capacity(clist, new_capacity)\
do\
{\
    if(clist)\
    {\
        const size_t n_bytes = ((new_capacity)*(clist_datasize(clist))) + (2*SIZEOF_ULONG);\
        size_t *ptr = reallocate(clist_start(clist), n_bytes);\
        (clist) = (void *) (&(ptr[2]));\
        __clist_set_capacity(clist, new_capacity);\
    }\
}while (0);


// we assume an external function macro increases the length of the list
// we check to see if the length has exceeded the capacity and if so, we grow...
#define clist_grow(clist)\
do{\
    if( (clist_length(clist)) > (clist_capacity(clist)) )\
    {\
        const size_t new_capacity = (size_t) (GROWTH_FACTOR * (clist_capacity(clist) + incrementor));\
        clist_change_capacity(clist, new_capacity);\
    }\
}while(0);\


//assume an external function macro decreases the length of the list.
// we check to see if the length has decreased beyond the Shrink factor * capacity
#define clist_shrink(clist)\
do{\
    if( ((clist_length(clist)) < (SHRINK_FACTOR*(clist_capacity(clist)))) && (clist_length(clist) > MIN_CAPACITY))\
    {\
        clist_change_capacity(clist, clist_length(clist));\
    }\
}while(0);


// increases the length by one
#define __clist_lengthen(clist)\
do{\
    const size_t new_length = ((size_t) (clist_length(clist))) + incrementor;\
    __clist_set_length(clist, new_length);\
    clist_grow(clist);\
}while(0);


//decreases length by one
#define __clist_shorten(clist)\
do{\
    size_t new_length = ((size_t) (clist_length(clist))) - incrementor;\
    __clist_set_length(clist, new_length);\
    clist_shrink(clist);\
}while(0);


//adds item to the end of the list
#define clist_append(clist, item)\
do{\
    __clist_lengthen(clist);\
    clist[(clist_length(clist))-1] = (item);\
}while(0);


// inserts item at given index, moving everything at index or further along the array
#define clist_insert(clist, item, index)\
do {\
    __clist_lengthen(clist);\
    assert(((size_t) index) <= (clist_length(clist)));\
    for (size_t i = ((clist_length(clist)) - 1); i > index; i--)\
    {\
        (clist)[i] = (clist)[i-1];\
    }\
    clist[index] = item;\
}while (0);\


// moves item at clist[index] into variable with name identifier
#define clist_remove(clist, identifier, index)\
do {\
    assert(((size_t) index) < (clist_length(clist)));\
    (identifier) = (clist)[(index)];\
    for (size_t i = (index); i < ((clist_length(clist)) - 1UL); i++)\
    {\
        (clist)[i] = (clist)[i+1UL];\
    }\
    __clist_shorten(clist);\
}while (0);\


// pushes an item to the end of the clist?
#define clist_push(clist, item)\
do{\
    __clist_lengthen(clist);\
    (clist)[(clist_length(clist)) - incrementor] = (item);\
}while(0);

//moves item from the end of the list into identifier
#define clist_pop(clist, identifier)\
do{\
    (identifier) = (clist)[(clist_length(clist)) - incrementor];\
    __clist_shorten(clist);\
}while(0);

//copies the clist

#endif
