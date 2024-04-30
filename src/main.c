#include <freelist.h>
#include <memalloc.h>
#include <stdio.h>
#include <stdlib.h>

int InitMyMalloc(int size) {
  /*initializes the heap and locks its size*/
  return heap_init((size_t)size, true);
}

void *MyMalloc(int size, int strategy) {
  Strategy strat = (Strategy)strategy;
  return memalloc_with_strategy((size_t)size, strat);
}

int MyFree(void *ptr) {
  memdealloc(ptr);
  return 0;
};

void DumpFreeList() { print_free_list(); }

Strategy get_strategy() {
  char buffer[1024];
  printf("Please enter strategy:\nBEST_FIT = 0\nFIT = 1\nFIRST_FIT = 2\nNEXT_FIT = 3\n");

  fgets(buffer, sizeof(buffer), stdin);

  Strategy strategy = (Strategy) atoi(buffer);

    return strategy;
}

int main() {
  InitMyMalloc(0);
  DumpFreeList();
  MyMalloc(200, NEXT_FIT);
  DumpFreeList();
}
