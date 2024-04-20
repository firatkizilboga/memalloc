#include <stdint.h>
#include <stddef.h>

#ifndef FLN_T
#define FLN_T
typedef struct{
  struct free_list_node *next;
  void *start;
  size_t size;
} free_list_node;

#endif

free_list_node *search_list(size_t size);
void free_list_init();
free_list_node *free_list_node_init(size_t size);
free_list_node *free_list_node_constructor(void *freedptr);
void coalesce();
void free_list_insert(free_list_node*);
free_list_node *get_free_list_tail();
void print_free_list();
