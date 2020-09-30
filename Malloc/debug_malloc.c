/*
** ETNA PROJECT, 16/01/2019 by soubri_j/martin_m
** debug_malloc.c
** File description: 
**      This file contains debug functions for malloc program.
*/

#include "malloc.h"

/*
    Method used to print one block
*/
void print_block(block_t *block)
{
    char *content = (char *)((block + 1));
    printf("Block : %p\n", block);
    printf("Real pointer : %p\n", (block + 1));
    printf("Available : %d\n", block->available);
    printf("Size : %lu\n", block->size);
    printf("Magic : %d\n", block->magic);
    printf("Next : %p\n", block->next);
    printf("Previous : %p\n", block->prev);
    printf("Content : %s\n", content);
}

/*
    Method used to display the chained list of blocks
*/
void print_list(void *first_block)
{
    block_t *ptr = NULL;

    if (first_block != NULL) {
        ptr = (((block_t *)first_block) - 1);
        printf("print_list\n");
        while (ptr != NULL) {
            print_block(ptr);
            ptr = ptr->next;
        }
    }
}