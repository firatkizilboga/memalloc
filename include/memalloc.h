#include <freelist.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

void *request_page(size_t size);
int heap_init(size_t size, bool in_lock);
void *memalloc(size_t size);
void memdealloc(void *ptr);
void *memalloc_with_strategy(size_t sizei, Strategy strategy);
