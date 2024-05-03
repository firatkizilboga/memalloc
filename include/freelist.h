#include <stddef.h>
#include <stdint.h>
#ifndef FLN_T
#define FLN_T
struct free_list_n {
  struct free_list_n *next;
  void *start;
  size_t size;
};
typedef struct free_list_n free_list_node;
enum strategyenum { BEST_FIT = 0, WORST_FIT = 1, FIRST_FIT = 2, NEXT_FIT = 3 };
typedef enum strategyenum Strategy;
#endif
free_list_node *free_list_search(size_t size, Strategy strategy);
int free_list_init(size_t size);
free_list_node *free_list_node_init(size_t size);
free_list_node *free_list_node_constructor(void *freedptr);
void coalesce();
void free_list_insert(free_list_node *);
void free_list_node_print(free_list_node *);
free_list_node *get_free_list_tail();
void print_free_list();
void unmap();
