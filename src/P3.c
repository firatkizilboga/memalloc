#include <stdio.h>
#include <unistd.h>  // For getpagesize()
#include <mymalloc.h>  // Custom memory management functions
#include <freelist.h>  // Free list data structure

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <strategy: 0, 1, 2, or 3>\n", argv[0]);
        return 1;
    }

    Strategy strategy;
    int input_strategy = atoi(argv[1]);
    switch (input_strategy) {
        case 0:
            strategy = BEST_FIT;
            break;
        case 1:
            strategy = WORST_FIT;
            break;
        case 2:
            strategy = FIRST_FIT;
            break;
        case 3:
            strategy = NEXT_FIT;
            break;
        default:
            fprintf(stderr, "Invalid strategy. Please choose 0, 1, 2, or 3.\n");
            return 1;
    }

    // Initialize memory management with multiple of page size
    size_t page_size = getpagesize();
    if (InitMyMalloc(2 * page_size) == -1) {  // 2 times the page size
        fprintf(stderr, "Failed to initialize memory.\n");
        return 1;
    }

    // Allocate memory using the chosen strategy
    void* ptrs[5];  // Reduced number of allocations for clarity
    for (int i = 0; i < 5; i++) {
        ptrs[i] = MyMalloc(page_size / 4, strategy);  // Allocate quarter page each time
        if (ptrs[i] == NULL) {
            fprintf(stderr, "Allocation failed at index %d\n", i);
            // Free any successful prior allocations
            for (int j = 0; j < i; j++) {
                MyFree(ptrs[j]);
            }
            return 1;
        }
    }
    
    DumpFreeList();  // Show the state of the heap after allocation


    // Simulate usage of allocated memory (if needed)

    // Deallocate all allocated memory
    for (int i = 0; i < 5; i++) {
        MyFree(ptrs[i]);
    }

    DumpFreeList();  // Show the state of the heap after deallocation
                     //
                     //
    MyMalloc(300, strategy);
    DumpFreeList();

    printf("All allocations and deallocations successful.\n");
    return 0;
}

