/*
* mm-naive.c - The fastest, least memory-efficient malloc package.
*
* In this naive approach, a block is allocated by simply incrementing
* the brk pointer.  A block is pure payload. There are no headers or
* footers.  Blocks are never coalesced or reused. Realloc is
* implemented directly using mm_malloc and mm_free.
*
* NOTE TO STUDENTS: Replace this header comment with your own header
* comment that gives a high level description of your solution.
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"


/*
make
./mdriver -f short2-bal.rep

*/
/*********************************************************
* NOTE TO STUDENTS: Before you do anything else, please
* provide your team information in the following struct.
********************************************************/
team_t team = {
  /* Team name */
  "Team Karasuno",
  /* First member's full name */
  "Benjamin Larkin",
  /* First member's email address */
  "larkinb3@wwu.edu",
  "",
  ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define BLK_HDR_SIZE ALIGN(sizeof(blockHdr))

typedef struct header blockHdr;
// doubly linked list header for explict list implementation
// 3 words, each 8 bytes large (alignment)
struct header {
  size_t size;
  blockHdr *next_p;
  blockHdr *prior_p;
};

void *find_fit(size_t size);
void coalesce(void* ptr);
void print_heap();

/*
* mm_init - initialize the malloc package.
*/
int mm_init(void)
{
  blockHdr *p = mem_sbrk(BLK_HDR_SIZE);
  p->size = BLK_HDR_SIZE;
  p->next_p = p;
  p->prior_p = p;
  return 0;
}

// iterate thru heap
void print_heap(){
  blockHdr *bp = mem_heap_lo();
  while (bp<(blockHdr*)mem_heap_hi()){
    printf("%s block at %p, size %d\n", (bp->size&1)?"allocated":"free", bp, (int)bp->size & ~1);
    bp = (blockHdr *)((char *)bp +( bp->size & ~1)); // get size without allocated bit
  }
}

/*
* mm_malloc - Allocate a block by incrementing the brk pointer.
*     Always allocate a block whose size is a multiple of the alignment.
*/
void *mm_malloc(size_t size){

  int newsize = ALIGN(BLK_HDR_SIZE + size);
  blockHdr *bp = find_fit(newsize);
  if(bp == NULL){
    bp = mem_sbrk(newsize);
    if (bp == (void*)-1) // no space
    return NULL;
    else
    bp->size = newsize | 1; // set size and set allocated
  }else {
    bp->size |= 1; // mark as allocated
    bp->prior_p->next_p = bp->next_p; // update pointers in list
    bp->next_p->prior_p = bp->prior_p;
  }
  return (char *)bp + BLK_HDR_SIZE; // byte based arithmetic on bp + size of header
}

// uses explicit list to find free block
void* find_fit(size_t size){
  blockHdr *p;
  // get first block in list, go to its next pointer
  for(p = ((blockHdr *)mem_heap_lo())->next_p;
  p != mem_heap_lo() && p->size < size; // if not enough space go to next
  p = p->next_p);

  if(p != mem_heap_lo())
  return p;

  else
  return NULL;
}

/*
* mm_free - Freeing a block does nothing.
*/
void mm_free(void *ptr)
{
  blockHdr* bp = ptr-BLK_HDR_SIZE  , *head = mem_heap_lo();
  bp->size &= ~1;
  bp->next_p = head->next_p;
  bp->prior_p = head->prior_p;
  head->next_p = bp;
  bp->next_p->prior_p = bp;
  // coalesce(bp);
}

void coalesce(void* ptr){

}

/*
* mm_realloc - Implemented simply in terms of mm_malloc and mm_free
*/
void *mm_realloc(void *ptr, size_t size)
{
  blockHdr *bp = ptr-BLK_HDR_SIZE;
  void *newptr = mm_malloc(size);
  if(newptr == NULL)
    return NULL;
  int copySize = bp->size-BLK_HDR_SIZE;
  if(size < copySize)
    copySize = size;
  memcpy(newptr, ptr, copySize);
  mm_free(ptr);
  return newptr;
}
