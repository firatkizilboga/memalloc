#include <freelist.h>
#include <memalloc.h>
#include <stdint.h>

static uintptr_t heap_base;
static uintptr_t heap_bound;

void *request_page(size_t size) {
  int page_size = getpagesize();
  int pages = size / page_size + 1;

  void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  heap_bound = (uintptr_t)ptr + page_size * pages;

  return ptr;
};

static bool is_initialized = false;
static bool is_locked = false;
int heap_init(size_t size, bool in_lock) {
  int res = free_list_init(size);
  if (res == 0) {
    free_list_node *head = get_free_list_tail();
    heap_base = (uintptr_t)head;
    is_initialized = true;
    is_locked = in_lock;
    return 0;
  }
  return -1;
};

void *memalloc_with_strategy(size_t sizei, Strategy strategy) {
  printf("lock: %d, init: %d\n", is_locked, is_initialized);
  if (!is_initialized && !is_locked) {

    heap_init(0, false);
  }     
  sizei = sizei + sizeof(size_t);
  free_list_node *target = free_list_search(sizei, strategy);
  if ((target == NULL || target->size < sizei) && !is_locked) {
    if (is_locked) { printf("ffed"); return NULL; }
    target = free_list_node_init(sizei);
    get_free_list_tail()->next = target;
}

  size_t *ptr = (size_t *)target->start;
  target->start = target->start + sizei;
  target->size -= sizei;

  *ptr = (size_t)sizei;

  return (void *)(ptr + 1);
}

void *memalloc(size_t size) { return memalloc_with_strategy(size, FIRST_FIT); }

void memdealloc(void *ptr) {
  void *actual_location = (void *)((size_t *)ptr - 1);
  size_t size = *((size_t *)actual_location);
  if (heap_bound > (uintptr_t)actual_location + size &&
      (uintptr_t)actual_location > heap_base) {

    free_list_node *node = free_list_node_constructor(actual_location);
    free_list_insert(node);
    coalesce();
  }
}
