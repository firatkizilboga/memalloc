#include <freelist.h>
#include <memalloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

static free_list_node *head = NULL;
int free_list_init(size_t size) {
  head = free_list_node_init(size);
  if (!head) {
    return -1;
  }
  return 0;
}

static free_list_node *first_fit(size_t size) {
  free_list_node *curr = head;
  while (curr) {
    if (curr->size >= size) {
      return curr;
    }
    curr = curr->next;
  }
  return NULL;
};

static free_list_node *best_fit(size_t size) {
  free_list_node *curr = head;
  free_list_node *best = NULL;
  while (curr) {
    if (curr->size >= size) {
      if (!best || curr->size < best->size) {
        best = curr;
      }
    }
    curr = curr->next;
  }
  return best;
};

static free_list_node *worst_fit(size_t size) {
  free_list_node *curr = head;
  free_list_node *worst = NULL;
  while (curr) {
    if (curr->size >= size) {
      if (!worst || curr->size > worst->size) {
        worst = curr;
      }
    }
    curr = curr->next;
  }
  return worst;
};

static free_list_node *next_fit(size_t size) {
  bool found = false;
  free_list_node *curr = head;
  while (curr) {
    if (curr->size >= size) {
      if (found) {
        return curr;
      }
      found = true;
    }
    curr = curr->next;
  }
  return NULL;
};

free_list_node *free_list_search(size_t size, Strategy strategy) {
  switch (strategy) {
  case FIRST_FIT:
    return first_fit(size);
  case BEST_FIT:
    return best_fit(size);
  case WORST_FIT:
    return worst_fit(size);
  case NEXT_FIT:
    return next_fit(size);
  default:
    return NULL;
  }
};

free_list_node *free_list_node_init(size_t size) {
  size_t size_new = size + sizeof(free_list_node);
  free_list_node *node = (free_list_node *)request_page(size_new);
  node->start = node + 1;
  node->next = NULL;
  node->size =
      (size_new / getpagesize() + 1) * getpagesize() - sizeof(free_list_node);

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
    if ((uintptr_t)curr->start + curr->size == (uintptr_t)curr->next) {
      curr->size += curr->next->size + sizeof(free_list_node);
      curr->next = curr->next->next;
      coalesce();
      return;
    } else if ((uintptr_t)curr->start + curr->size ==
               (uintptr_t)curr->next->start) {
      // this is the case where there is an inner block until the bigger blocks
      // end is freed
      curr->next->size += curr->size + sizeof(free_list_node);
      curr->next->start = curr->start - sizeof(free_list_node);
      curr->size = 0;

      coalesce();
      return;
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

free_list_node *get_free_list_tail() {
  free_list_node *curr = head;
  while (curr && curr->next) {
    curr = curr->next;
  };

  return curr;
};

int calculate_empty(free_list_node *node) {
  /* returns 1 if nonempty, 0 if empty */
  return node + 1 == node->start;
}

void print_free_list() {
  free_list_node *curr = head;
  if (!curr) {
    printf("\n");
  }
  void *base = curr->start;

  printf("%-12s %-10s %-10s\n", "Addr", "Size", "Status");
  while (curr) {
    printf("0x%-10x %-10zu %-10s\n",
           (unsigned int)((uintptr_t)curr->start - (uintptr_t)base), curr->size,
           calculate_empty(curr) ? "Empty" : "Full");
    curr = curr->next;
  }
};
