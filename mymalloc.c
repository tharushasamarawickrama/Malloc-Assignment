#include "mymalloc.h"


#define MEMORY_SIZE 25000


static char memory[MEMORY_SIZE];

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;


static Block* free_list = NULL;


static void splitBlock(Block* block, size_t size) {
    if (block->size > size + sizeof(Block)) {
        Block* newBlock = (Block*)((char*)block + size + sizeof(Block));
        newBlock->size = block->size - size - sizeof(Block);
        newBlock->next = block->next;
        block->size = size;
        block->next = newBlock;
    }
}


static void initializeMemory() {
    free_list = (Block*)memory;
    free_list->size = MEMORY_SIZE - sizeof(Block);
    free_list->next = NULL;
}

void* MyMalloc(size_t size) {
    if (free_list == NULL) {
       
        initializeMemory();
    }

    
    Block* curr = free_list;
    Block* prev = NULL;

    while (curr != NULL) {
        if (curr->size >= size) {
            if (prev != NULL) {
                prev->next = curr->next;
            } else {
                free_list = curr->next;
            }

            splitBlock(curr, size);

            return (void*)((char*)curr + sizeof(Block));
        }

        prev = curr;
        curr = curr->next;
    }

    return NULL; 

void MyFree(void* ptr) {
    if (ptr == NULL) {
        return;  
    }

    
    Block* block = (Block*)((char*)ptr - sizeof(Block));

    
    block->next = free_list;
    free_list = block;
}

