#include <easyalloc.h>
#include <unistd.h>
#include <freelist.h>

int main() {

  /* Proof of coalescing, reallocation and unmapping*/
  size_t page_size = getpagesize();
  InitMyMalloc(page_size - sizeof(free_list_node));
  DumpFreeList();

  free_list_node *node1 = MyMalloc(50, 0);
  MyFree(node1);
  DumpFreeList();

  MyMalloc(50, 0);
}

