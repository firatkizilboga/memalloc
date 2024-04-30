#include <easyalloc.h>
#include <unistd.h>
#include <freelist.h>

int main() {

  /* Proof of coalescing and reallocation */
  size_t page_size = getpagesize();
  printf("Page size: %ld\n", page_size);
  InitMyMalloc(page_size - sizeof(free_list_node));
  DumpFreeList();

  void*ptr = MyMalloc(100, 0);
  void*ptr2 = MyMalloc(100, 0);


  DumpFreeList();
  MyFree(ptr2);
  DumpFreeList();

  MyFree(ptr);
  DumpFreeList();

  MyMalloc(100, 0);
  DumpFreeList();
}
