/*
** ETNA PROJECT, 10/01/2019 by soubri_j/martin_m
** malloc.c
** File description: 
**      This file contains malloc program.
*/

#include "malloc.h"

block_t *first_block_in_memory = NULL;
block_t *last_block_in_memory = NULL;
void *lowest_address = NULL;

/*
    Method to allocate space in memory.
    Parameters :
        size : Size to allocate
    Returns :
        A block_t pointer to the allocated space if ok, NULL otherwise.
*/
block_t *allocate_memory_space(size_t size)
{
    block_t *to_return = NULL;
    void *current_heap = NULL;

    current_heap = sbrk(0);
    initialize_memory(&first_block_in_memory, &lowest_address, current_heap);
    if (sbrk(size + sizeof(*to_return)) != (void *)-1) {
        to_return = current_heap;
        initialize_block(to_return, size, 0);
        if (last_block_in_memory != NULL) {
            to_return->prev = last_block_in_memory;
            last_block_in_memory->next = to_return;
        } else {
            if (to_return != first_block_in_memory) {
                to_return->prev = first_block_in_memory;
            }
        }
        last_block_in_memory = to_return;
    }
    return (to_return);
}

/*
    Method used to find an available block in the list of blocks
    The function looks for an available block from the start and from the
    end at the same time.
    When an available block is found, it is merge with next available if 
    possible and then if the size of the block is greater or equal to
    the seeked size, it returns the block found
    Parameters :
        size : Minimum size of a block to find
    Returns :
        The block if was found, NULL otherwise.
*/
block_t *find_available_block(size_t size)
{
    block_t *ptr_start = NULL;
    block_t *ptr = NULL;
    int block_found = 0;

    if (first_block_in_memory != NULL) {
        ptr_start = first_block_in_memory;
    }
    if (ptr_start != NULL) {
        while (ptr_start != NULL && block_found == 0) {
            if (ptr_start->available == 1 && ptr_start->size >= size) {
                block_found = 1;
                ptr = ptr_start;
            } else {
                ptr_start = ptr_start->next;
            }
        }
    }
    return (ptr);
}

/*
    Malloc function. Allocate memory size.
    Parameter :
        size : Size to allocate
*/
void *malloc(size_t size)
{
    block_t *block = NULL;
    void *to_return = NULL;

    if (first_block_in_memory == NULL) {
        block = allocate_memory_space(size);
    } else {
        block = find_available_block(size);
        if (block == NULL) {
            block = allocate_memory_space(size);
        }
    }
    if (block != NULL) {
        block->available = 0;
        block = split_block(block, size, &last_block_in_memory);
        to_return = (block + 1);
    }
    return (to_return);
}

/*
    Free allocated memory.
    Parameters :
        ptr : The pointer to free
*/
void free(void *ptr)
{
    block_t *block = NULL;

    if (check_pointer(ptr, lowest_address) == 1) {
        block = ((block_t *)ptr - 1);
        block->available = 1;
    }
}

/*
    Method to enlarge memory allocation. If there is enough space
    it enlarge size of the allocated space, otherwise it allocates new
    memory space and free the old one.
    Parameters : 
        ptr : The pointer to enlarge
        size : New size
    Returns :
        A pointer to the allocated space.
*/
void *realloc(void *ptr, size_t size)
{
    block_t *block = NULL;
    size_t size_to_copy = 0;
    void *to_return = ptr;
    if (ptr != NULL) {
        block = (((block_t *)ptr) - 1);
        if (check_pointer(ptr, lowest_address) == 1) {
            size_to_copy = block->size;
            block = merge_blocks(block, &last_block_in_memory);
            if (block->size >= size) {
                to_return = realloc_copy_mem(ptr, block, size, size_to_copy);
                block = split_block(block, size, &last_block_in_memory);
            } else {
                to_return = malloc(size);
                to_return = memmove(to_return, ptr, MIN(size_to_copy, size));
                free(ptr);
            }
        }
    } else {
        to_return = malloc(size);
    }
    return (to_return);
}

/*
    The calloc function contiguously allocates enough space for count
    objects that are size bytes of memory each and returns a pointer to the
    allocated memory.  The allocated memory is filled with bytes of value
    zero.
    Parameters : 
        count : Number of block to allocate
        size : Size of each blocks
    Returns :
        A pointer to allocated memory if ok, NULL otherwise.
*/
void *calloc(size_t count, size_t size)
{
    size_t size_to_find = 0;
    block_t *block = NULL;
    void *to_return = NULL;
    void *res_malloc = NULL;

    if (count > 0 && size > 0) {
        size_to_find = (count * size);
        res_malloc = malloc(size_to_find);
        if (res_malloc != NULL) {
            block = (block_t *)(res_malloc - sizeof(block_t));
            fill_block_with_zeroes(block);
        }
    }
    if (block != NULL) {
        to_return = (block + 1);    
    }
    return (to_return);
}
