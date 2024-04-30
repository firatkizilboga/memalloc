#include <freelist.h>
#include <memalloc.h>
#include <stdio.h>
#include <stdlib.h>

int InitMyMalloc(int size); 
void *MyMalloc(int size, int strategy); 
int MyFree(void *ptr);
void DumpFreeList(); 
Strategy get_strategy(); 
