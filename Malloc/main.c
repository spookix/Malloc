#ifndef __MALLOC_H__
# define __MALLOC_H__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

#define MIN(x,y) (((x) < (y)) ? (x) : (y))

/* Constants */
static const int magic_number = 0x77777777;

/* Error messages */
static const char error_pointer_too_low[] = "Pointer address is too low";
static const char error_wrong_pointer[] = "Pointer address is not correct";

/* Structures definition */
typedef struct block_s {
    struct block_s* next;
    struct block_s* prev;
    size_t size;
    int available;
    int magic;
} block_t;


block_t *first_block_in_memory = NULL;
block_t *last_block_in_memory = NULL;
void *lowest_address = NULL;

int main()
{
    float *ptr1 = NULL, *ptr2 = NULL;
    int i, n = 5, termination = 1;

    ptr1 = calloc(n, sizeof(float)); /* On alloue de la mémoire avec calloc() pour ptr1. */
    if (ptr1 == NULL)
        printf("Echec de calloc().\n");
    else {
        ptr2 = mallocS(n * sizeof(float));/* On alloue de la mémoire avec malloc() pour ptr2. */
        if (ptr2 == NULL)
            printf("Echec de malloc().\n");
        else {
            printf("Allocation de memoire avec\n");
            printf("calloc() pour ptr1\n");
            printf("malloc() pour ptr2.\n\n");
            for (i=0; i<n; i++)
                printf("ptr1[%d]=%f, ptr2[%d]=%f\n", i, ptr1[i], i, ptr2[i]);
            /* On affiche la valeur de chacune des cases des pointeurs. */
            printf("\n");
            free(ptr2);
            termination = 0;
        }
        free(ptr1);
    }

    return termination;
}


void *mallocS(size_t size)
{
    block_t *block = NULL;
    void *to_return = NULL;

    printf("\nc'est bien ici\n");
    if (first_block_in_memory == NULL) {
        block = allocate_memory_space(size);
    } else {
        block = find_available_block(size);
        if (block == NULL) {
            block = allocate_memory_space(size);
        } else {
            block->available = 0;
            block = split_block(block, size);
        }
    }
    if (block != NULL) {
        to_return = (block + 1);
    }

    return to_return;
}


/*
    Method to allocate space in memory.
    Parameters :
        size : Size to allocate
    Returns :
        A block_t pointer to the allocated space if ok, NULL otherwise.
*/
block_t *allocate_memory_space(size_t size) {
    block_t *to_return = NULL;
    void *current_heap = NULL;

    current_heap = sbrk(0);
    if (first_block_in_memory == NULL) {
        lowest_address = current_heap;
        first_block_in_memory = current_heap;
    }
    if (sbrk(size + sizeof(*to_return)) != (void*)-1) {
        to_return = current_heap;
        to_return->size = size;
        to_return->next = NULL;
        to_return->available = 0;
        if (last_block_in_memory != NULL) {
            to_return->prev = last_block_in_memory;
            last_block_in_memory->next = to_return;
        } else {
            if (to_return != first_block_in_memory) 
            {
                to_return->prev = first_block_in_memory;
            }
        }
        to_return->magic = magic_number;
        
        last_block_in_memory = to_return;
    }

    return to_return;
}
