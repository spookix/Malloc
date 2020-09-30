/*
** ETNA PROJECT, 16/01/2019 by soubri_j/martin_m
** optimize_malloc.c
** File description: 
**      This file contains optimizing functions for malloc program.
*/

#include "malloc.h"

/*
    Method used to split a block in two block if all the size is not used.
    We check that the space left have at least a size of 1 + block_t size.
    If it is the case we split the block in two blocks.
    Parameters :
        block_to_split : The block to split
        used_size : Used size in the block to split
        last_block_in_memory : The last sbrk allocated block in memory
    Returns :
        The block to split.
*/
block_t *split_block(block_t *block_to_split, size_t used_size,
                        block_t **last_block_in_memory)
{
    block_t *new_block = NULL;

    if (block_to_split != NULL) {
        if (used_size > 0 && 
            (block_to_split->size - used_size >= sizeof(block_t) + 1)) {
            new_block = (block_t *)(((void *)block_to_split) + sizeof(block_t)
                + used_size);
            new_block->prev = block_to_split;
            new_block->next = block_to_split->next;
            new_block->size = block_to_split->size - used_size 
                - sizeof(block_t);
            new_block->available = 1;
            new_block->magic = magic_number;
            block_to_split->size = used_size;
            block_to_split->next = new_block;
            if (new_block->next != NULL) {
                new_block->next->prev = new_block;
            } else {
                (*last_block_in_memory) = new_block;
            }
        }
    }
    return (block_to_split);
}

/*
    Method to initialize block with 0.
    Parameters :
        block : Block to initialize
    Returns :
        The initialized block
*/
block_t *fill_block_with_zeroes(block_t *block) 
{
    void *memory = NULL;
    char *content = NULL;
    size_t size = 0;

    if (block != NULL) {
        size = block->size;
        memory = (void *)(block + 1);
        content = (char *)memory;
        while (size--) {
            *content = '\0';
            content++;
        }
    }

    return (block);
}

/*
    Method used to copy memory in realloc process
    Parameters :
        ptr : Pointer to realloc
        block : Pointer to the ptr block
        size : Size to allocate
        to_copy : Size to copy
    Returns :
        A pointer to the new copied memory address
*/
void *realloc_copy_mem(void *ptr, block_t *block, size_t size, size_t to_copy)
{
    void *to_return = NULL;
    if ((block + 1) != ptr) {
        to_return = memmove((block + 1), ptr, MIN(to_copy, size));
    } else {
        to_return = (block + 1);
    }
    block->available = 0;
    return (to_return);
}