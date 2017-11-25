/* malloc.c: simple memory allocator -----------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Macros --------------------------------------------------------------------*/

#define ALIGN4(s)           (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)       ((b) + 1)
#define BLOCK_HEADER(ptr)   ((struct block *)(ptr) - 1)

/* Block structure -----------------------------------------------------------*/

struct block {
    size_t        size;
    struct block *next;
    bool          free;
};

/* Global variables ----------------------------------------------------------*/

struct block *FreeList = NULL;
struct block *NEXT     = NULL;

/* Add counters for mallocs, frees, reuses, grows */
bool IF_AT_EXIT_DEF = false;
int  N_MALLOCS      = 0;
int  N_FREES        = 0;
int  N_REUSES       = 0;
int  N_GROWS        = 0;
int  N_SPLITS       = 0;
int  N_COALESCES    = 0;
int  N_BLOCKS       = 0;
int  N_REQUESTED    = 0;
int  N_MAXHEAP      = 0;

/* atexit --------------------------------------------------------------------*/

void report_counters() {
    char buffer[BUFSIZ] = {0};
    sprintf(buffer,
        "mallocs:   %i\n"
        "frees:     %i\n"
        "reuses:    %i\n"
        "grows:     %i\n"
        "splits:    %i\n"
        "coalesces: %i\n"
        "blocks:    %i\n"
        "requested: %i\n"
        "max heap:  %i\n",
        N_MALLOCS, N_FREES, N_REUSES, N_GROWS, N_SPLITS, N_COALESCES, N_BLOCKS, N_REQUESTED, N_MAXHEAP);
    write(1, buffer, strlen(buffer));
    close(1);
}

/* Find free block -----------------------------------------------------------*/

struct block *find_free(struct block **last, size_t size) {
    struct block *curr = FreeList;

/* First fit */
#if defined FIT && FIT == 0
    while (curr && !(curr->free && curr->size >= size)) {
        *last = curr;
        curr  = curr->next;
    }
#endif

/* Next fit */
#if defined FIT && FIT == 1
    // Update curr to the saved next pointer if next is not NULL.
    if (NEXT) {
        struct block *original = NEXT;
        curr = NEXT;      
        // Iterate to end of list.
        while (curr && !(curr->free && curr->size >= size)) {
            *last = curr;
            curr  = curr->next;
        }
        // If no free block was found, curr is NULL.
        if (curr == NULL) {
            curr = FreeList; // Set curr equal to beginning of list.
            while (curr && !(curr->free && curr->size >= size)) {
                if (curr == original) {
                    while (curr) { *last = curr; curr  = curr->next; } // Update last pointer to grow_heap().
                    NEXT = NULL;
                    break;
                }
                *last = curr;
                curr  = curr->next;
            }
        }
        NEXT = curr; // Update NEXT pointer.
    }
    else { // Perform first fit because there is no next pointer.
        while (curr && !(curr->free && curr->size >= size)) {
            *last = curr;
            curr  = curr->next;
        }
        // Update the pointer.
        NEXT = curr;
    }
#endif

/* Best fit */
#if defined FIT && FIT == 2
    struct block *best = NULL;
    // Find first block whose size is big enough and is free.
    while (curr) {
        if (curr->free && curr->size >= size) {
            best = curr;
            *last = curr;
            curr  = curr->next;
            break;
        }

        *last = curr;
        curr  = curr->next;
    }
    if (best) {
        while (curr) {
            if (curr->free && curr->size >= size) {
                if (curr->size < best->size)
                    best = curr;
            }

            *last = curr;
            curr  = curr->next;
        }
    }

    curr = best;
#endif

/* Worst fit */
#if defined FIT && FIT == 3
    struct block *best = NULL;
    // Find first block whose size is big enough and is free.
    while (curr) {
        if (curr->free && curr->size >= size) {
            best = curr;
            *last = curr;
            curr  = curr->next;
            break;
        }

        *last = curr;
        curr  = curr->next;
    }
    if (best) {
        while (curr) {
            if (curr->free && curr->size >= size) {
                if (curr->size > best->size) 
                    best = curr;
            }

            *last = curr;
            curr  = curr->next;
        }
    }
    curr = best;
#endif

    N_REUSES++;

    return curr;
}

/* Grow heap -----------------------------------------------------------------*/

struct block *grow_heap(struct block *last, size_t size) {
    /* Request more space from OS */
    struct block *curr = (struct block *)sbrk(0);
    struct block *prev = (struct block *)sbrk(sizeof(struct block) + size);

    assert(curr == prev);

    /* OS allocation failed */
    if (curr == (struct block *)-1) {
        return NULL;
    }

    /* Update FreeList if not set */
    if (FreeList == NULL) {
        FreeList = curr;
    }

    /* Attach new block to prev block */
    if (last) {
        last->next = curr;
    }

    /* Update block metadata */
    curr->size = size;
    curr->next = NULL;
    curr->free = false;

    N_GROWS++; 
    N_BLOCKS++;
    N_MAXHEAP += size;

    return curr;
}

/* Allocate space ------------------------------------------------------------*/

void *malloc(size_t size) {
    /* atexit */
    atexit(report_counters);

    /* Align to multiple of 4 */
    size = ALIGN4(size);

    /* Handle 0 size */
    if (size == 0) {
        return NULL;
    }

    /* Look for free block */
    struct block *last = FreeList;
    struct block *next = find_free(&last, size);

    /* Could not find free block, so grow heap */
    if (next == NULL) {
        next = grow_heap(last, size);
        
        N_REUSES--;    
    }

    /* Could not find free block or grow heap, so just return NULL */
    if (next == NULL) {
        return NULL;
    }
  
    /* Splitting block into perfect size */
    if (next->size > size + sizeof(struct block)) { 
        size_t new_size = next->size - size - sizeof(struct block); 
        struct block *new = (struct block *)((size_t)next + sizeof(struct block) + size); 
        new->size = new_size;
        new->next = next->next;
        new->free = true;

        next->size = size;
        next->next = new;
        next->free = false;
        
        N_SPLITS++;
        N_BLOCKS++;
    }

    /* Mark block as in use */
    next->free = false;

    N_MALLOCS++;
    N_REQUESTED += size;

    /* Return data address associated with block */
    return BLOCK_DATA(next);
}

/* Coalesce blocks -----------------------------------------------------------*/
void coalesce(struct block *curr) {
    /* Iterate through free list */
    struct block *next = curr->next;

    if (next) {
        if (curr->free == true && next->free == true) {
            size_t new_size = curr->size + next->size + sizeof(struct block);
            curr->size = new_size;
            curr->next = next->next;
            
            N_COALESCES++;
            N_BLOCKS--;
        }
    }
}

/* Reclaim space -------------------------------------------------------------*/

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    /* Make block as free */
    struct block *curr = BLOCK_HEADER(ptr);
    assert(curr->free == 0);
    curr->free = true;
    
    N_FREES++; 

    /* Coalesce free blocks */
    coalesce(curr);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=cpp: --------------------------------*/
