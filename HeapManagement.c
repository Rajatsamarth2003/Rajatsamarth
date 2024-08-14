#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 1000

// Structure representing a block in the heap
typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

// Global pointer to the start of the heap
Block* heap_start = NULL;

// Function to initialize the heap
void initialize_heap() {
    heap_start = (Block*)malloc(HEAP_SIZE);
    if (heap_start == NULL) {
        fprintf(stderr, "Heap initialization failed\n");
        exit(EXIT_FAILURE);
    }
    heap_start->size = HEAP_SIZE - sizeof(Block);
    heap_start->next = NULL;
}

// Function to allocate memory from the heap
void* allocate(size_t size) {
    if (heap_start == NULL) {
        initialize_heap();
    }

    // Find a suitable free block
    Block* current = heap_start;  // Pointer to the current block being checked
    Block* previous = NULL;

    while (current != NULL) {
        if (current->size >= size) {
            // Allocate from this block
            if (current->size > size + sizeof(Block)) {
                // Split the block if it's larger than needed
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }

            // Mark the block as allocated
            return (void*)(current + 1);
        }

        previous = current;
        current = current->next;
    }

    // No suitable block found
    fprintf(stderr, "Failed to allocate memory\n");
    return NULL;
}

// Function to free memory in the heap
void deallocate(void* ptr) {
    if (ptr == NULL) {
        return; // Ignore NULL pointers
    }

    // Mark the block as free
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->next = NULL;

    // Merge adjacent free blocks
    Block* current = heap_start;
    Block* previous = NULL;

    while (current != NULL) {
        if (current->next == block) {
            // Merge with the next free block
            current->size += sizeof(Block) + block->size;
            block = current;
            break;
        } else if (block->next == current) {
            // Merge with the previous free block
            block->size += sizeof(Block) + current->size;
            block->next = current->next;
            if (previous != NULL) {
                previous->next = block;
            } else {
                heap_start = block;
            }
            break;
        }

        previous = current;
        current = current->next;
    }
}

// Function to print the state of the heap
void print_heap_state() {
    Block* current = heap_start;
    while (current != NULL) {
        printf("Block: %p, Size: %zu\n", (void*)current, current->size);
        current = current->next;
    }
}

int main() {
    initialize_heap();

    int choice;
    size_t size;
    void* block;

    do {
        printf("\n1. Allocate memory\n");
        printf("2. Free memory\n");
        printf("3. Print heap state\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the size to allocate: ");
                scanf("%zu", &size);
                block = allocate(size);
                if (block != NULL) {
                    printf("Memory allocated at address: %p\n", block);
                }
                break;

            case 2:
                printf("Enter the address to free: ");
                scanf("%p", &block);
                deallocate(block);
                printf("Memory freed at address: %p\n", block);
                break;

            case 3:
                printf("Current state of the heap:\n");
                print_heap_state();
                break;

            case 0:
                printf("Exiting program\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
