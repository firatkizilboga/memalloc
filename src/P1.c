#include <stdio.h>
#include <unistd.h>  // For getpagesize()d
#include <easyalloc.h>  // Assume this includes InitMyMalloc, MyMalloc, MyFree
#include <freelist.h>  // Assume this includes structures for managing the free list

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
    if (InitMyMalloc(4 * page_size) == -1) {  // Initialize with 20 pages
        fprintf(stderr, "Memory initialization failed.\n");
        return 1;
    }

    //Prove when we free that we can coalesce and we can reuse the memory

    // Allocate several blocks
    printf("Allocating 2 blocks of page size and two page sizes respectively \n");

    void* ptr1 = MyMalloc(page_size, strategy);
    void* ptr2 = MyMalloc(2 * page_size, strategy);
    printf("Initial allocations:\n");
    DumpFreeList();

    // Deallocate middle blocks to create adjacent free blocks
    MyFree(ptr1);
    printf("After freeing first block:\n");
    DumpFreeList();

    // Allocate a block that fits into the coalesced space
    void* ptr3 = MyMalloc(190, strategy);
    printf("After allocation in the coalesced space:\n");

    DumpFreeList();

    // Freeing ptr2
    MyFree(ptr2);
    printf("After freeing second block:\n");
    DumpFreeList();
    return 0;
}

