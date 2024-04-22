#include <freelist.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

static free_list_node *head = NULL;
void free_list_init() { head = free_list_node_init(0); }

free_list_node *search_list(size_t size) {
  /*Returns a pointer to free list node if a greater or equal sized node is
   * found else NULL*/
  free_list_node *curr = head;

  while (curr) {
    if (curr->size >= size) {
      return curr;
    }

    curr = curr->next;
  }
  return curr;
};

void print_free_list_node(free_list_node *node) {
  printf("Printing node location 0x%x : size %d : start 0x%x : end 0x%x\n",
         node, node->size, node->start, (uintptr_t)node->start + node->size);
};

extern free_list_node *request_page(size_t);
free_list_node *free_list_node_init(size_t size) {
  size_t size_new = size + sizeof(free_list_node);
  free_list_node *node = (free_list_node *)request_page(size_new);
  node->start = node + 1;
  node->next = NULL;
  node->size =
      (size_new / getpagesize() + 1) * getpagesize() - sizeof(free_list_node);

  print_free_list_node(node);
  return node;
};

free_list_node *free_list_node_constructor(void *freedptr) {
  size_t size = *((size_t *)freedptr);
  free_list_node *node = (free_list_node *)freedptr;
  node->size = size - sizeof(free_list_node);
  node->start = (void *)(node + 1);
  node->next = NULL;
  return node;
};

void coalesce() {
  free_list_node *curr = head;
  // change head to the first non zero size
  if (head->size == 0) {
    head = head->next;
  }

  while (curr && curr->next) {
    if (curr->next->size == 0) {
      curr->next = curr->next->next;
      continue;
    }

    if ((uintptr_t)curr->start + curr->size == (uintptr_t)curr->next) {
      curr->size += curr->next->size + sizeof(free_list_node);
      curr->next = curr->next->next;
      coalesce();
    } else if ((uintptr_t)curr->start + curr->size ==
               (uintptr_t)curr->next->start) {
      // this is the case where there is an inner block until the bigger blocks
      // end is freed
      curr->next->size += curr->size + sizeof(free_list_node);
      curr->next->start = curr->start - sizeof(free_list_node);
      curr->size = 0;

      coalesce();
    }
    curr = curr->next;
  }
};

void free_list_insert(free_list_node *node) {
  if (!head || (uintptr_t)node->start < (uintptr_t)head->start) {

    node->next = head;
    head = node;
  } else {
    // Traverse the list and find the correct spot
    free_list_node *current = head;
    while (current->next &&
           (uintptr_t)current->next->start < (uintptr_t)node->start) {
      current = current->next;
    }
    node->next = current->next;
    current->next = node;
  }
}

void free_list_node_print(free_list_node *node) {
  printf("Printing node location 0x%x : size %d : start 0x%x\n", node,
         node->size, node->start);

  fflush(stdout);
};

free_list_node *get_free_list_tail() {
  free_list_node *curr = head;
  while (curr && curr->next) {
    curr = curr->next;
  };

  return curr;
};

void print_free_list() {
  free_list_node *curr = head;
  printf("###########FREELIST#########\n");
  if (!curr) {
    printf("\n");
  }

  while (curr) {
    printf("location 0x%x : size %d : start 0x%x, end: 0x%x\n", curr,
           curr->size, curr->start, (uintptr_t)curr->start + curr->size);
    curr = curr->next;
  }
};
