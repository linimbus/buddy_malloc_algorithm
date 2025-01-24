#ifndef __MIN_MALLOC__
#define __MIN_MALLOC__

#include <stddef.h>

#define __unused 0    /*  Free flag  */
#define __used   1    /*  Used flag  */

typedef struct _buddy_system_ {
    int flag; // Flag to indicate if the space is used
    int kval; // Indicates the size of the space, 2^n
    struct _buddy_system_ *next; // Doubly linked list
	struct _buddy_system_ *pres;
}Buddy,*PBuddy;

struct _head_node_ {
	size_t nodesize;
	PBuddy head;
};

void displayFreeList(void);  // Print information of all nodes in the free list
int min_init(void* start, unsigned long size); // Initialize memory space
void* min_malloc(size_t  size); // Memory allocation
void min_free(void *p); // Memory deallocation
void min_uninit();

#endif