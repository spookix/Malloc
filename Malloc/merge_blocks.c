/*
** ETNA PROJECT, 31/01/2019 by soubri_j/martin_m
** merge_blocks.c
** File description: 
**      This file contains merge blocks functions for malloc program.
*/

#include "malloc.h"

/*
    Method used to merge block_to_optimize with its previous block
    Parameters :
        block_to_optimize : The block to merge
    Returns
        The block to optimize
*/
block_t *merge_previous_block(block_t *block_to_optimize)
{
    if (block_to_optimize != NULL) {
        if (block_to_optimize->prev != NULL) {
            if (block_to_optimize->prev->available == 1) {
                block_to_optimize->prev->size += block_to_optimize->size 
                    + sizeof(*block_to_optimize);
                block_to_optimize->prev->next = block_to_optimize->next;
                if (block_to_optimize->next != NULL) {
                    block_to_optimize->next->prev = block_to_optimize->prev;
                }
                block_to_optimize = block_to_optimize->prev;
            }
        }
    }
    return (block_to_optimize);
}

/*
    Method used to merge block_to_optimize with its next block
    Parameters :
        block_to_optimize : The block to merge
    Returns
        The block to optimize
*/
block_t *merge_next_block(block_t *block_to_optimize)
{
    block_t *to_merge = NULL;

    if (block_to_optimize != NULL) {
        if (block_to_optimize->next != NULL) {
            if (block_to_optimize->next->available == 1) {
                to_merge = block_to_optimize->next;
                if (block_to_optimize->next->next != NULL) {
                    block_to_optimize->next->next->prev = block_to_optimize;
                }
                block_to_optimize->size += to_merge->size + sizeof(*to_merge);
                block_to_optimize->next = block_to_optimize->next->next;
            }
        }
    }
    return (block_to_optimize);
}

/*
    Method used to optimize size of a block. In case of reusing a block,
    we look at previous and next block to see if they are available.
    If they are we merge blocks in one, allocate the size, and split the block
    in two if there is space left.
    Parameters :
        block_to_optimize : The block to optimize
        last_block_in_memory : The last sbrk allocated block in memory
    Returns :
        An merged block if possible, the block_to_optimize otherwise
*/
block_t *merge_block(block_t *block_to_optimize, 
                    block_t **last_block_in_memory)
{
    block_to_optimize = merge_previous_block(block_to_optimize);
    block_to_optimize = merge_next_block(block_to_optimize);

    if (block_to_optimize->next == NULL) {
        (*last_block_in_memory) = block_to_optimize;
    }
    return (block_to_optimize);
}

/*
    Method used to merge blocks while contiguous block are available
    Paramters :
        block_to_optimize : Pointer to an adress to optimize
        last_block_in_memory : The last sbrk allocated block.
    Returns ;
        The new address if optimization has been done, the old one otherwise.
*/
block_t *merge_blocks(block_t *block_to_optimize, 
                    block_t **last_block_in_memory)
{
    size_t old_size = 0;
    block_t *block = block_to_optimize;

    do {
        old_size = block->size;
        block = merge_block(block, last_block_in_memory);
    } while (old_size != block->size);

    return (block);
}