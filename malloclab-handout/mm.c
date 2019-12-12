/* Benjamin Larkin
 * CSCI 410
 * Dynamic Memory Allocator utilizing:
 * Doubly linked list
 * Permanent 'empty' list-head 
 * Explicit free list structure
 * LIFO policy for freed blocks
 *
 * Free blocks
 |HDR ------------- FTR|

 * Alloc Blocks
 |HDR ---PAYLOAD--- FTR|

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

#define ALIGNMENT 8

// this fixes allocated bit at the end so we don't end up with 2 or 3 after adding sizes together
#define FIX_BIT(x) (x & ~1)

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
void mm_check();
int checkAddress(blockHdr* bh);
int checkSize(blockHdr* bh);

/* incomplete function which merges adjacent free blocks
 * first it checks if left and right are free
 * then it updates next/prev pointers and fixes the sizes
*/
void *coalesce(blockHdr *oldBlock)
{
  blockHdr *newBP = NULL;
      int leftFree = 0;  // check footer of previous block
      int rightFree = 0; // check header of next block
      int flagL = 0;
      int flagR = 0;
      int currSize = oldBlock->size;
      blockHdr *nextBlock;
      blockFtr *nextBlockFtr;
      printf("rightCheck: %p \nmemHi: %p\n", (char *)oldBlock + currSize, (char *)mem_heap_hi());
      if ((((char *)oldBlock) + currSize) < (char *)mem_heap_hi()) // avoid segfault
      {
          nextBlock = (blockHdr *)((char *)oldBlock) + currSize;
          nextBlockFtr = (blockFtr *)((char *)nextBlock) + ((nextBlock->size) - BLK_FTR_SIZE);
          flagR = 1;
      }
      blockFtr *prevBlockFtr;
      printf("leftCheck: %p \nmemLo: %p\n", (char *)oldBlock - BLK_HDR_SIZE, (char *)mem_heap_lo());
      if ((((char *)oldBlock)) - BLK_HDR_SIZE != (char *)mem_heap_lo())
      {
          prevBlockFtr = (blockFtr *)((char *)oldBlock) - BLK_FTR_SIZE;
          flagL = 1;
      }

      if (flagR)
      {
          if (nextBlock->size & ~1)
          {
              rightFree = 1;
          }
      }
      if (flagL)
      {

          if (prevBlockFtr->size & ~1)
          {
              leftFree = 1;
          }
      }
      // 4 cases
      if (!rightFree && !leftFree)
      {
          return (void *)oldBlock;
      }
      if (rightFree && leftFree)
      {
          blockHdr *rightBlock = nextBlock;
          blockHdr *leftBlock = rightBlock->prior_p;
          newBP = leftBlock;
          newBP->next_p = rightBlock->next_p;
          newBP->prior_p = leftBlock->prior_p; // redundant?
          int newTotalSize = (FIX_BIT(oldBlock->size) + FIX_BIT(rightBlock->size) + FIX_BIT(leftBlock->size)) | 1;
          newBP->size = newTotalSize;
          blockFtr *newBPftr = (blockFtr *)((char *)newBP + newBP->size - BLK_FTR_SIZE);
          newBPftr->size = newTotalSize;
      }
      if (rightFree && !leftFree)
      {
          int newTotalSize = ((FIX_BIT(oldBlock->size) + FIX_BIT(nextBlock->size))) | 1;
          newBP->size = newTotalSize;        //-BLK_HDR_SIZE -BLK_FTR_SIZE) | 1;// double counting size?
                                             // accounting for 2 headers and footers?
          newBP->next_p = nextBlock->next_p; //where nextBlock is the free block on the right
          newBP->prior_p = nextBlock->prior_p;
          nextBlockFtr->size = newTotalSize;
      }
      if (!rightFree && leftFree)
      {
          int leftSize = ((blockFtr *)(oldBlock - BLK_FTR_SIZE))->size;
          newBP = (blockHdr *)((char *)oldBlock) - leftSize; //((char *) oldBlock) + currSize;
          newBP->size = ((FIX_BIT(oldBlock->size) + FIX_BIT(leftSize))) | 1;
          // move footer right
          blockFtr *bf = (blockFtr *)((char *)newBP + newBP->size - BLK_FTR_SIZE); // set footer size and allocate
          bf->size = newBP->size;
      }
      return (void *)newBP;
}

int mm_init(void)
{
    blockHdr *p = mem_sbrk(BLK_HDR_SIZE);
    p->size = BLK_HDR_SIZE;
    p->next_p = p;
    p->prior_p = p;
    return 0;
}

/* iterate thru heap and check the following:
 * print allocated/free blocks
 * check if address is within the heap range
 * check if header and footer sizes match
*/
void mm_check()
{
    blockHdr *bp = mem_heap_lo();
    while (bp < (blockHdr *)mem_heap_hi())
    {
        printf("%s block at %p, size %d\n",
               (bp->size & 1) ? "allocated" : "free",
               bp,
               (int)(bp->size & ~1));
        bp = (blockHdr *)((char *)bp + (bp->size & ~1));
        if(checkAddress(bp)){
          printf("\npointer address out of heap");
        }
        if(checkSize(bp)){
          printf("\nsize discrepency");
        }
    }

}

// checks if address is within the bounds of the heap
int checkAddress(blockHdr* bh){
  char* bhAddress = (char*) bh;
  if(bhAddress > (char*)mem_heap_hi() || bhAddress < (char*)mem_heap_lo()){
    return 1;
  }
  return 0;
}

// checks if header and footer sizes match
int checkSize(blockHdr* bh){
  int currSize = bh->size;
  blockFtr *bf = (blockFtr* ) ((char*)bh + currSize - BLK_FTR_SIZE);
  if(bh->size == bf->size){
    return 0;
  }
  return 0;

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

/* frees the address in memory by setting the allocated bit to 1
 * then adds newly freed block to head of list in LIFO order
*/
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

/*
 * simple wrapper around mm_malloc and mm_free
*/
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
