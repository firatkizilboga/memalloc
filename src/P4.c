#include <stdio.h>
#include <unistd.h>  
#include <easyalloc.h>  
#include <freelist.h>  

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <strategy: 0, 1, 2, or 3>\n", argv[0]);
        return 1;
    }

    int strategy = atoi(argv[1]);
    if (strategy < 0 || strategy > 3) {
        fprintf(stderr, "Invalid strategy. Please choose between 0 (Best Fit) and 3 (Next Fit).\n");
        return 1;
    }

    size_t page_size = getpagesize();
    if (InitMyMalloc(10 * page_size) == -1) {  // Initialize with 10 pages
        fprintf(stderr, "Memory initialization failed.\n");
        return 1;
    }

    // Allocate several blocks
    void* ptr1 = MyMalloc(page_size, strategy);
    void* ptr2 = MyMalloc(2 * page_size, strategy);
    void* ptr3 = MyMalloc(page_size, strategy);
    void* ptr4 = MyMalloc(2 * page_size, strategy);

    // Show initial memory state
    printf("Initial allocations:\n");
    DumpFreeList();

    // Deallocate middle blocks to create adjacent free blocks
    MyFree(ptr2);
    MyFree(ptr3);
    printf("After freeing middle blocks:\n");
    DumpFreeList();

    // Allocate a block that fits into the coalesced space
    void* ptr5 = MyMalloc(3 * page_size, strategy);
    printf("After allocation in the coalesced space:\n");
    DumpFreeList();

    // Final cleanup
    MyFree(ptr1);
    MyFree(ptr4);
    MyFree(ptr5);
    printf("Final state after all deallocations:\n");
    DumpFreeList();

    return 0;
}

