#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <freelist.h>


static free_list_node *head = NULL;
void free_list_init(){
   head = free_list_node_init(0); 
}

free_list_node *search_list(size_t size) {
    /*Returns a pointer to free list node if a greater or equal sized node is found else NULL*/
    free_list_node* curr = head;

    while(curr){
        if(curr->size >= size){
            return curr;
        }
        
        curr = curr->next;
    }
    return curr;
};

extern free_list_node* request_page(size_t);
free_list_node *free_list_node_init(size_t size) {
    size_t size_new = size + sizeof(free_list_node);
    free_list_node* node = (free_list_node*) request_page(size_new);
    node->start = node+1;
    node->next = NULL;
    node->size = (size_new / getpagesize() + 1) * getpagesize() - sizeof(free_list_node*);
    return node;
};

free_list_node *free_list_node_constructor(void *freedptr) {
    size_t size = *((size_t*)freedptr);
    free_list_node* node = (free_list_node*) freedptr;
    node->size = size - sizeof(free_list_node);
    node->start = (void*)(node + 1);
    node->next = NULL;

};


void coalesce() {

};


void free_list_insert(free_list_node *node) {

};

free_list_node *get_free_list_tail() {
    free_list_node*curr = head;
    while(curr && curr->next){
        curr = curr->next;
    };

    return curr;
};


void print_free_list(){
    free_list_node*curr = head;
    printf("###########FREELIST#########\n");
    if(!curr){printf("\n");}

    while (curr) {
        printf(
            "location 0x%x : size %d : start 0x%x\n", curr, curr->size, curr->start
        );
        curr = curr->next;
    }
    
};
