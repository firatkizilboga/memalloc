#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <easyalloc.h>
#include <freelist.h>

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

    size_t page_size = getpagesize();
    InitMyMalloc(page_size - sizeof(free_list_node));

    // Perform mallocs and frees according to the selected strategy
    const int num_allocations = 10;
    void* ptrs[num_allocations];

    for (int i = 0; i < num_allocations; i++) {
        ptrs[i] = MyMalloc(100, strategy);
        if (ptrs[i] == NULL) {
            fprintf(stderr, "Allocation failed at index %d\n", i);
            // Free all previous allocations before returning
            for (int j = 0; j < i; j++) {
                MyFree(ptrs[j]);
            }
            return 1;
        }
    }

    // Free all allocations
    for (int i = 0; i < num_allocations; i++) {
        MyFree(ptrs[i]);
    }

    printf("All allocations and deallocations successful.\n");
    return 0;
}
