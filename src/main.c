#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	struct free_list_node * next;
	void* start;
	size_t size;
} free_list_node;

static free_list_node * head = NULL;

free_list_node* search_list(size_t size){
	free_list_node* curr = head;
	while(curr && curr->size < size){
		curr = curr->next;
	}

	return curr;
};

void* request_page(size_t size){
	int page_size = getpagesize();
	int pages = size / page_size + 1;

	void* ptr = mmap(
		NULL, size, 
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1, 0
	);

	return ptr;
};

free_list_node* free_list_node_init(size_t size){
	free_list_node* new = (free_list_node*) request_page(sizeof(free_list_node) + size);
	new->next = NULL;
	
	int pages = (sizeof(free_list_node)+size)/getpagesize() + 1;
	new->size = getpagesize() * pages - sizeof(free_list_node);
	new->start = (void*) (new + 1);
	return new;
};

void heap_init(){
	head = (free_list_node*) free_list_node_init(0);
};

free_list_node* get_free_list_tail(){
	free_list_node* curr = head;
	while (curr->next)
	{
		curr = curr->next;
	}
	return curr;
	
}

void* memalloc(size_t sizei) {
    sizei = sizei + sizeof(size_t);
    free_list_node* target = search_list(sizei);

    if (target == NULL || target->size < sizei) {
        target = free_list_node_init(sizei);
		get_free_list_tail()->next = target;
    }

    size_t* ptr = (size_t*)target->start;
    target->start = target->start + sizei;
    target->size -= sizei;

	*ptr = (int)sizei;
	return (void*)(ptr+1);
}


void print_free_list(){
	free_list_node* curr = head;
	while(curr) {
		printf("START: 0x%x SIZE: %zu\n", curr->start, curr->size);
		curr = curr->next;
	}
};

int main(){
	heap_init();
	print_free_list();
	int* ptr = (int*) memalloc(sizeof(int));
	*ptr = 20;
	printf("assigned value: %d\n",*ptr);
	(int*) memalloc(sizeof(int)*20);
	print_free_list();
	printf("assigned value: %d\n",*ptr);

	int* arr = (int*) memalloc(sizeof(int)*991);
	print_free_list();
	printf("assigned value: %d\n",*ptr);

	memalloc(sizeof(int)*20);
	print_free_list();

}
