/* Benjamin Larkin
 * CSCI 410
 * Dynamic Memory Allocator utilizing: 
 * Doubly linked list
 * Explicit free list structure  
 * LIFO policy for freed blocks
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

#define ALIGNMENT 8

// round up to the nearest multiple of alignment
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define BLK_HDR_SIZE ALIGN(sizeof(blockHdr))

#define BLK_FTR_SIZE ALIGN(sizeof(blockFtr))

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Team_Karasuno",
    /* First member's full name */
    "Benjamin Larkin",
    /* First member's email address */
    "larkinb3@wwu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

typedef struct header blockHdr;
typedef struct footer blockFtr;

// 3 words, each 8 bytes large (alignment)
struct header
{
    size_t size;
    blockHdr *next_p;
    blockHdr *prior_p;
};

struct footer
{
    size_t size;
};

void *find_fit(size_t size);
void *coalesce(blockHdr *oldBlock);
void print_heap();

void *coalesce(blockHdr *oldBlock)
{
    // check if previous block free
    // check if next block free
    // combine sizes?
    int leftFree = 0;  // check footer of previous block
    int rightFree = 0; // check header of next block
}

int mm_init(void)
{
    blockHdr *p = mem_sbrk(BLK_HDR_SIZE);
    p->size = BLK_HDR_SIZE;
    p->next_p = p;
    p->prior_p = p;
    return 0;
}

// iterate thru heap
void print_heap()
{
    blockHdr *bp = mem_heap_lo();
    while (bp < (blockHdr *)mem_heap_hi())
    {
        printf("%s block at %p, size %d\n",
               (bp->size & 1) ? "allocated" : "free",
               bp,
               (int)(bp->size & ~1));
        bp = (blockHdr *)((char *)bp + (bp->size & ~1));
    }
}

void *mm_malloc(size_t size)
{
    int newsize = ALIGN(BLK_HDR_SIZE + size + BLK_FTR_SIZE);
    blockHdr *bp = find_fit(newsize);
    // footer arithmatic
    if (bp == NULL)
    {
        bp = mem_sbrk(newsize);
        if ((long)bp == -1) // no space
            return NULL;
        else
            bp->size = newsize | 1;                     // set size and allocated bit
        blockFtr *bf = (blockFtr *)((char *)bp + size); // set footer size and allocate
        bf->size = newsize | 1;
    }
    else
    {
        bp->size |= 1;
        // set footer size and allocate
        blockFtr *bf = (blockFtr *)((char *)bp + size); // set footer size and allocate
        bf->size = newsize | 1;
        bp->prior_p->next_p = bp->next_p; // update pointers in the list
        bp->next_p->prior_p = bp->prior_p;
    }
    return (char *)bp + BLK_HDR_SIZE; // byte based arithmetic on bp + size of header
}

// uses explicit list to find first free block
void *find_fit(size_t size)
{
    blockHdr *p;
    for (p = ((blockHdr *)mem_heap_lo())->next_p; // get first block in list and go to its next pointer
         p != mem_heap_lo() && p->size < size;
         p = p->next_p)
        ; // if not enough space, go next

    if (p != mem_heap_lo())
        return p;
    else
        return NULL;
}

void mm_free(void *ptr)
{
    blockHdr *bp = ptr - BLK_HDR_SIZE,
             *head = mem_heap_lo();
    bp->size &= ~1;
    // footer arithmetic size &= ~1
    bp->next_p = head->next_p; // LIFO  policy
    bp->prior_p = head;
    head->next_p = bp;
    bp->next_p->prior_p = bp;
    // coalesce(bp);
}

void *mm_realloc(void *ptr, size_t size)
{
    blockHdr *bp = ptr - BLK_HDR_SIZE;
    void *newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    int copySize = bp->size - BLK_HDR_SIZE;
    if (size < copySize)
        copySize = size;
    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    return newptr;
}
