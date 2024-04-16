#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct {
  struct free_list_node *next;
  void *start;
  size_t size;
} free_list_node;

static free_list_node *head = NULL;

free_list_node *search_list(size_t size) {
  free_list_node *curr = head;
  while (curr && curr->size < size) {
    curr = curr->next;
  }

  return curr;
};

void *request_page(size_t size) {
  int page_size = getpagesize();
  int pages = size / page_size + 1;

  void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  return ptr;
};

free_list_node *free_list_node_init(size_t size) {
  free_list_node *new =
      (free_list_node *)request_page(sizeof(free_list_node) + size);
  new->next = NULL;

  int pages = (sizeof(free_list_node) + size) / getpagesize() + 1;
  new->size = getpagesize() * pages - sizeof(free_list_node);
  new->start = (void *)(new + 1);

  if (head && head > new) {
    new->next = head;
    head = new;
    return new;
  }

  return new;
};

free_list_node *free_list_node_constructor(void *freedptr) {
  size_t s = *((size_t *)freedptr);
  free_list_node *new = (free_list_node *)freedptr;
  printf("SIZE: %zu\n", s);
  new->next = NULL;
  new->size =
      s - sizeof(free_list_node); // Assign the size from the block's metadata
  new->start = (void *)(new + 1); // Point to the memory area right after the
                                  // metadata/node struct
  return new;
};

void coalesce() {
  free_list_node *curr = head;
  while (curr->next) {
    free_list_node *n = curr->next;
    if (curr->start + curr->size == n) {
      curr->size += n->size + sizeof(free_list_node);
      curr->next = n->next;
    } else {
      curr = n;
    }
  }
};

void free_list_insert(free_list_node *new) {
  free_list_node *curr = head;
  if (curr > new) {
    new->next = curr;
    head = new;
    coalesce();
    return;
  }

  while (curr->next && curr->start > new->start) {
    curr = curr->next;
  }
  new->next = curr->next;
  curr->next = new;
  coalesce();
};

void heap_init() { head = (free_list_node *)free_list_node_init(0); };

free_list_node *get_free_list_tail() {
  free_list_node *curr = head;
  while (curr->next) {
    curr = curr->next;
  }
  return curr;
}

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

void print_free_list() {
  free_list_node *curr = head;

  printf("###############################\nFREE LIST: \n");
  while (curr) {
    curr->size > 0
        ? printf(
              "SELF 0x%x,START: 0x%x SIZE: %zu NEXT LOCATION: 0x%x OR 0x%x\n",
              curr, curr->start, curr->size, curr->next,
              curr->start + curr->size)
        : 0;
    curr = curr->next;
  }
};

void memdealloc(void *ptr) {
  void *actual_location = (void *)((size_t *)ptr - 1);
  free_list_node *node = free_list_node_constructor(actual_location);
  free_list_insert(node);
}

int main() {
  printf("sizeof(free_list_node): %zu\n", sizeof(free_list_node));
  int *n = (int *)memalloc(sizeof(int) * 16);
  print_free_list();

  int *m = (int *)memalloc(sizeof(int) * 16);

  print_free_list();

  int *o = (int *)memalloc(sizeof(int) * 16);

  print_free_list();

  int *p = (int *)memalloc(sizeof(int) * 16);

  print_free_list();

  memdealloc(m);
  memdealloc(o);
  memdealloc(p);
  memdealloc(n);

  print_free_list();
}
