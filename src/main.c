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
  return new;
};

free_list_node *free_list_node_constructor(void *freedptr) {
  printf("freedptr: 0x%x\n", freedptr);
  // print the size_t value at the pointed location
  size_t s = *((size_t *)freedptr);
  printf("size of freedptr: %d\n", s);
  free_list_node *new = (free_list_node *)freedptr;

  new->next = NULL;
  new->size =
      s - sizeof(free_list_node); // Assign the size from the block's metadata
  printf("size of freedptr: %d\n", new->size);
  new->start = (void *)(new + 1); // Point to the memory area right after the
                                  // metadata/node struct
  return new;
};

void free_list_insert(free_list_node *new) {
  free_list_node *curr = head;

  while (curr->next && curr->start < new->start) {
    curr = curr->next;
  }
  new->next = curr->next;
  curr->next = new;
  // coalesce();
};

void heap_init() { head = (free_list_node *)free_list_node_init(0); };

free_list_node *get_free_list_tail() {
  free_list_node *curr = head;
  while (curr->next) {
    curr = curr->next;
  }
  return curr;
}

void *memalloc(size_t sizei) {
  sizei = sizei + sizeof(size_t);
  free_list_node *target = search_list(sizei);

  if (target == NULL || target->size < sizei) {
    target = free_list_node_init(sizei);
    get_free_list_tail()->next = target;
  }

  size_t *ptr = (size_t *)target->start;
  target->start = target->start + sizei;
  target->size -= sizei;

  printf("allocated pointer location: 0x%x\n", ptr);
  *ptr = (size_t)sizei;
  printf("size of allocated pointer: %zu\n", *ptr);

  return (void *)(ptr + 1);
}

void print_free_list() {
  free_list_node *curr = head;

  printf("###############################\n");
  while (curr) {
    printf("START: 0x%x SIZE: %zu\n", curr->start, curr->size);
    curr = curr->next;
  }
};

void memdealloc(void *ptr) {
  printf("passed pointer location: 0x%x\n", ptr);
  void *actual_location = (void *)((size_t *)ptr - 1);

  printf("actual location of pointer: 0x%x\n", actual_location);
  free_list_node *node = free_list_node_constructor(actual_location);
  free_list_insert(node);
}

int main() {
  printf("size of free list node: %zu\n", sizeof(free_list_node));
  heap_init();
  print_free_list();
  int *ptr = (int *)memalloc(sizeof(int));
  *ptr = 20;
  printf("assigned value: %d\n", *ptr);

  ptr = (int *)memalloc(sizeof(int) * 20);
  print_free_list();

  int *arr = (int *)memalloc(sizeof(int) * 991);
  print_free_list();

  memalloc(sizeof(int) * 20);
  print_free_list();

  memdealloc(ptr);
  print_free_list();
}
