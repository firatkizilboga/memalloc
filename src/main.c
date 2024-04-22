#include <freelist.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

void *request_page(size_t size) {
  int page_size = getpagesize();
  int pages = size / page_size + 1;

  void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  return ptr;
};

void heap_init() { free_list_init(); };

static bool is_initialized = false;
void *memalloc(size_t sizei) {
  if (!is_initialized) {
    heap_init();
    is_initialized = true;
  }
  sizei = sizei + sizeof(size_t);
  free_list_node *target = search_list(sizei);

  if (target == NULL || target->size < sizei) {
    target = free_list_node_init(sizei);
    get_free_list_tail()->next = target;
  }

  size_t *ptr = (size_t *)target->start;
  target->start = target->start + sizei;
  target->size -= sizei;

  *ptr = (size_t)sizei;

  return (void *)(ptr + 1);
}

void memdealloc(void *ptr) {
  void *actual_location = (void *)((size_t *)ptr - 1);
  free_list_node *node = free_list_node_constructor(actual_location);
  free_list_insert(node);

  coalesce();
}

int main() {
  printf("sizeof(free_list_node): %zu\n", sizeof(free_list_node));
  printf("sizeof size_t: %zu\n", sizeof(size_t));

  memalloc(1);
  print_free_list();
  int *n = (int *)memalloc(sizeof(int) * 16);
  int *m = (int *)memalloc(sizeof(int) * 200);
  int *p = (int *)memalloc(sizeof(int) * 200);

  memdealloc(m);

  memdealloc(n);

  memdealloc(p);

  memdealloc(q);
  print_free_list();
}
