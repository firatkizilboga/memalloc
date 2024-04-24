#include <freelist.h>
#include <memalloc.h>

int InitMyMalloc(int size) { return heap_init((size_t)size); }

void *MyMalloc(int size, int strategy) {
  Strategy strat = (Strategy)strategy;
  return memalloc_with_strategy((size_t)size, strat);
}

int MyFree(void *ptr) {
  memdealloc(ptr);
  return 0;
};

void DumpFreeList() { print_free_list(); }

int main() {
  InitMyMalloc(0);
  void *ptr = MyMalloc(sizeof(int) * 16, FIRST_FIT);
  void *ptr1 = MyMalloc(160, FIRST_FIT);
  DumpFreeList();
  MyFree(ptr);
  DumpFreeList();
  MyMalloc(20, NEXT_FIT);
  DumpFreeList();
 
  MyFree(ptr1 + 18000);
  DumpFreeList();
}
