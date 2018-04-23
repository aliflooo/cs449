// Aliyah Flores
// aff13

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include "mymalloc.h"

// Don't change or remove these constants.
#define MINIMUM_ALLOCATION  16
#define SIZE_MULTIPLE       8

// Added these constants
#define LARGEST_BIN_SIZE 512 
#define SIZE_OF_USED offsetof(Node, previous_free)
#define MIN_SIZE (MINIMUM_ALLOCATION + SIZE_OF_USED)
#define NUMBER_OF_BINS (2 + ((512 - MINIMUM_ALLOCATION)/SIZE_MULTIPLE))
#define OVERFLOW (NUMBER_OF_BINS - 1)

typedef struct Node {
  struct Node *previous_block; 
  struct Node *next_block; 
  unsigned int size; 
  int in_use; 
  struct Node* previous_free; 
  struct Node* next_free; 
} Node;

Node* head = NULL; 
Node* tail = NULL; 
Node* bins[NUMBER_OF_BINS] = {};

// Prototypes
Node* splitting(Node* block, int);
Node* coalescing(Node* block);
void inserting(Node* block);
void removing(Node* block);

unsigned int round_up_size(unsigned int data_size)
{
  if(data_size == 0)
    return 0;
  else if(data_size < MINIMUM_ALLOCATION)
    return MINIMUM_ALLOCATION;
  else
    return (data_size + (SIZE_MULTIPLE - 1)) & ~(SIZE_MULTIPLE - 1);
}

unsigned int bin_index_num(unsigned int block) { 
  return (round_up_size(block) - MINIMUM_ALLOCATION) / SIZE_MULTIPLE;
}

void* new_ptr(void* ptr, int byte) { 
  return (void*)(((char*)ptr) + byte);
}

void* my_malloc(unsigned int size)
{
  if(size == 0)
    return NULL;
  
  size = round_up_size(size);
  
  unsigned int index = bin_index_num(size);
  Node* current = bins[index]; 
  Node* new_allocation = NULL;
  Node* split = NULL; 
  
  if (current != NULL && index < OVERFLOW) { 
    new_allocation = current;
    new_allocation->in_use = 1; 
    removing(current);
  } 
  else if(current == NULL && index < OVERFLOW) { 
    for (int i = bin_index_num(size + MIN_SIZE); i < OVERFLOW; i++) {
      if(bins[i] != NULL) {
        split = bins[i];
        if((split->size - size) < MIN_SIZE) {
          split = NULL;
          continue;
        } 
        
        removing(split);
        break;
      }
    }
    
    if (split != NULL) { 
      new_allocation = splitting(split, size);
    }
      
  }
  
  if(new_allocation == NULL) {
    current = bins[OVERFLOW];
    
    while (current != NULL) {
      if (current->size >= size) {
        if(((current->size  - size) >= MIN_SIZE)) {
          split = current;
          new_allocation = splitting(split, size);
        } 
        else {
          new_allocation = current;
          new_allocation->in_use = 1;
          removing(new_allocation);
        }
        break;
      } 
      else {
        current = current->next_free;
      }
    }
  }
  
  if (new_allocation == NULL) { 
    new_allocation = sbrk(size + SIZE_OF_USED);
    new_allocation->size = size;
    new_allocation->in_use = 1;
    
    if(tail == NULL) {
      new_allocation->previous_block = NULL;
      new_allocation->next_block = NULL;
      tail = new_allocation;
    } else {
      Node* temp = tail;
      temp->next_block = new_allocation;
      new_allocation->next_block = NULL;
      new_allocation->previous_block = temp;
      tail = new_allocation;
    } 
  }
  return (void*) new_ptr(new_allocation, SIZE_OF_USED);
}


void removing(Node* block) { 
  unsigned int index = bin_index_num(block->size);
  
  if (block->next_free != NULL && block->previous_free != NULL) {
    Node* temp_prev = block->previous_free;
    Node* temp_next = block->next_free;
    temp_prev->next_free = temp_next;
    temp_next->previous_free = temp_prev;
  } 
  else if(block->next_free == NULL && block->previous_free == NULL) { 
  //if block is the only one left in the list
    bins[index] = NULL;
  } 
  else if (block->next_free != NULL && block->previous_free == NULL) {
  //if block is the head but not the only block
    bins[index] = block->next_free;
    block->next_free->previous_free = NULL;
  } 
  else if (block->next_free == NULL && block->previous_free != NULL) {
  //if block is at the end of the list
    Node* hold = block->previous_free;
    hold->next_free = NULL;
  }
}

void inserting(Node* block) { //inserting block 
  unsigned int index = bin_index_num(block->size);
  
  if(bins[index] == NULL) {
  //if there is nothing 
    block->previous_free = NULL;
    block->next_free = NULL;
    bins[index] = block;
  } else {
    Node* temp = bins[index];
    temp->previous_free = block;
    block->next_free = temp;
    block->previous_free = NULL;
    bins[index] = block;
  }
}

Node* splitting(Node* block, int alloc_size) {
  int previous_size = block->size;
  
  removing(block);
  
  Node* allo_block = block;
  Node* free = new_ptr(block, alloc_size + SIZE_OF_USED);
  Node* next = block->next_block;
  allo_block->in_use = 1;
  allo_block->size = alloc_size;
  
  free->in_use = 0;
  free->size = (previous_size - alloc_size) - SIZE_OF_USED;
  
  //put together allo_block with free
  allo_block->next_block = free;
  free->previous_block = allo_block;
  
  //put together empty with the next
  free->next_block = next;
  
  if(next != NULL) {
    next->previous_block = free;
  } else {
    tail = free;
  }
  //put free part back into bin
  inserting(free);
  return allo_block;
}

Node* coalescing(Node* block) {
  if(block->previous_block != NULL && block->next_block != NULL) {
    //check its surroundings
    if(block->previous_block->in_use == 0 && block->next_block->in_use == 0) {
      //coalescing together because both previous and next blocks are not in_use
      removing(block->previous_block);
      removing(block->next_block);
      //all three block's sizes
      int new_size = block->previous_block->size + (block->size + SIZE_OF_USED) + (block->next_block->size + SIZE_OF_USED);
      Node* coalescing = block->previous_block;
      coalescing->size = new_size;
      coalescing->in_use = 0; //coalesced block is free
      
      coalescing->next_block = block->next_block->next_block;
      
      if(coalescing->next_block == NULL) {
        tail = coalescing;
      } else {
        coalescing->next_block->previous_block = coalescing;
      }
      return coalescing;
    } 
    else if(block->previous_block->in_use == 0 && block->next_block->in_use == 1) {
      removing(block->previous_block);
      int new_size = block->previous_block->size + (block->size + SIZE_OF_USED);
      Node* coalescing = block->previous_block;
      coalescing->size = new_size;
      coalescing->in_use = 0;
      
      coalescing->next_block = block->next_block;
      coalescing->next_block->previous_block = coalescing;
      return coalescing;
    } 
    else if(block->previous_block->in_use == 1 && block->next_block->in_use == 0) {
      removing(block->next_block);
      
      int new_size = block->next_block->size + (block->size + SIZE_OF_USED);
      Node* coalescing = block;
      coalescing->size = new_size;
      coalescing->in_use = 0;
      
      coalescing->next_block = block->next_block->next_block;
      
      if(coalescing->next_block == NULL) {
        tail = coalescing;
      } else {
        coalescing->next_block->previous_block = coalescing;
      }
      return coalescing;
    } 
  } 
  else if(block->previous_block != NULL && block->next_block == NULL) {
    if(block->previous_block->in_use == 0) {
      removing(block->previous_block);
      
      int new_size = block->previous_block->size + (block->size + SIZE_OF_USED);
      Node* coalescing = block->previous_block;
      coalescing->size = new_size;
      coalescing->in_use = 0;
      
      coalescing->next_block = NULL;
      tail = coalescing;
      return coalescing;
    } 
  } 
  else if (block->previous_block == NULL && block->next_block != NULL) {
    if(block->next_block->in_use == 0) {
      removing(block->next_block);
      
      int new_size = block->next_block->size + (block->size + SIZE_OF_USED);
      Node* coalescing = block;
      coalescing->size = new_size;
      coalescing->in_use = 0;
      
      coalescing->next_block = block->next_block->next_block;
      
      if(coalescing->next_block == NULL) {
        tail = coalescing;
      } else {
        coalescing->next_block->previous_block = coalescing;
      }
      return coalescing;
    }
  }
  return block;
}

void my_free(void* ptr)
{
  if(ptr == NULL)
    return;
  
  Node* freed = (Node*)new_ptr(ptr, -SIZE_OF_USED);
  freed = coalescing(freed);
  if (freed->next_block == NULL) {
    if (tail == NULL) {
    } 
    else if(tail->previous_block == NULL) {
      tail = NULL;
    } 
    else {
      Node* temp = tail->previous_block;
      temp->next_block = NULL;
      tail = temp;
    }
    brk(freed);
  } 
  else {
    freed->in_use = 0;
    inserting(freed);
  }
}