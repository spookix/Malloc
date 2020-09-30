/*
** ETNA PROJECT, 11/01/2019 by soubri_j/martin_m
** malloc.h
** File description: 
**      This file contains headers for malloc program.
*/

#ifndef __MALLOC_H__
# define __MALLOC_H__
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define MIN(x,y) (((x) < (y)) ? (x) : (y))

/* Structures definition */
typedef struct block_s {
    struct block_s* next;
    struct block_s* prev;
    size_t size;
    int available;
    int magic;
} block_t;

/* Constants */
static const int magic_number = 0x77777777;

/* Error messages */
static const char error_pointer_too_low[] = "Pointer address is too low";
static const char error_wrong_pointer[] = "Pointer address is not correct";

/* Methods declaration */
void* mallocS(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void print_list(void *first_block);
void print_block(block_t *block);
block_t *fill_block_with_zeroes(block_t *block);
void initialize_block(block_t *to_initialize, size_t size, int available);
void initialize_memory(block_t **first_block, void **lowest, void *heap);
void *realloc_copy_mem(void *ptr, block_t *block, size_t size, size_t to_copy);
block_t *merge_block(block_t *block_to_optimize, 
                    block_t **last_block_in_memory);
block_t *merge_blocks(block_t *block_to_optimize, 
                    block_t **last_block_in_memory);
block_t *split_block(block_t *block_to_split, size_t used_size, 
                    block_t **last_block_in_memory);
#endif
