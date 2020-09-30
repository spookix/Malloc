/*
** ETNA PROJECT, 19/01/2019 by soubri_j/martin_m
** check.c
** File description: 
**      This file contains check methods for malloc program.
*/

#include "malloc.h"

/*
    Method used to check pointer validity
    Parameters :
        ptr : The pointer to check
        lowest_adress : The lowest address in memory
    Returns :
        0 if pointer is not valid, 1 otherwise.
*/
int check_pointer(void *ptr, void *lowest_address)
{
    int iReturn = 0;
    block_t *block = NULL;

    if (ptr != NULL) {
        if (lowest_address != NULL && ptr < lowest_address) {
            printf("%s\n", error_pointer_too_low);
        } else {
            block = (((block_t *)ptr) - 1);
            if (block->magic != magic_number) {
                printf("%s\n", error_wrong_pointer);
            } else {
                iReturn = 1;
            }
        }
    }
    return (iReturn);
}

/*
    Method to initialize a block with default values
    Parameters :
        to_initialize : The block to initialize
        size : Size of the block
        available : Is the block available
*/
void initialize_block(block_t *to_initialize, size_t size, int available)
{
    if (to_initialize != NULL) {
        to_initialize->size = size;
        to_initialize->available = available;
        to_initialize->next = NULL;
        to_initialize->magic = magic_number;
        to_initialize->prev = NULL;
    }
}

/*
    Method used to initialize memory
    Parameters :
        first_block : The first block of memory to initialize
        lowest : The lowest memory address used to check pointer sanity
        heap : The current heap
*/
void initialize_memory(block_t **first_block, void **lowest, void *heap)
{
    if (*first_block == NULL) {
        *lowest = heap;
        *first_block = heap;
    }
}