#include "min_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int M_Val;                       // Maximum memory block size allocated 2^M
struct _head_node_ FreeList[32]; // Free list of nodes
void *MallocAddress;             // Starting address of allocated memory

void disone(PBuddy one) // Print information of a node
{
  printf("flag = %d  Kval = %d  pres = %p  next = %p  \n", one->flag, one->kval,
         one->pres, one->next);
}

void displayFreeList() // Print information of all nodes in the free list
{
  PBuddy one;
  int i;
  for (i = 0; i <= M_Val; i += 1) {
    printf("No %d %d ", FreeList[i].nodesize, i);
    one = FreeList[i].head;
    if (one) {
      do {
        printf("flag = %d  Kval = %d  pres = %p  next = %p ", one->flag,
               one->kval, one->pres, one->next);
        one = one->next;
      } while (one != FreeList[i].head);
    }
    printf("\n");
  }
}

void InsertHead(PBuddy pone) // Insert a node at the head of the doubly linked list
{
  PBuddy head = FreeList[pone->kval].head; // Save the existing head node
  pone->flag = __unused; // The node inserted into the free list is unused
  if (head != NULL)      // Case where there is already a head node
  {
    head->pres->next = pone;
    pone->pres = head->pres;
    head->pres = pone;
    pone->next = head;
  } else // Case where there is no head node
  {
    pone->next = pone;
    pone->pres = pone;
  }
  FreeList[pone->kval].head = pone; // Update the head node
}

PBuddy DeleteHead(int kval) // Delete the head node
{
  PBuddy phead = FreeList[kval].head; // Save the head node
  if (phead != phead->next)           // Case where there are more than one head nodes
  {
    phead->pres->next = phead->next;
    phead->next->pres = phead->pres;
    FreeList[kval].head = phead->next;
  } else // Case where there is one or no head node
    FreeList[kval].head = NULL;
  return phead;
}

void DeleteNode(PBuddy pone) // Delete a node
{
  if (FreeList[pone->kval].head == pone) // Case where the node is the head node
    DeleteHead(pone->kval);
  else // Case where the node is not the head node
  {
    pone->pres->next = pone->next;
    pone->next->pres = pone->pres;
  }
}

void InitFreeList(size_t max_size) // Initialize the free list
{
  M_Val = 0;                     // Initialize the maximum free block size to 2^0
  FreeList[M_Val].nodesize = 16; // Initialize the minimum free block size to 16 bytes
  FreeList[M_Val].head = NULL;   // Initialize the head pointer to NULL
  for (; FreeList[M_Val].nodesize <= max_size;
       M_Val +=
       1) { // Calculate all 16-byte to 2^m (maximum allocatable power-of-two space), and initialize the corresponding head pointers to NULL
    FreeList[M_Val + 1].nodesize = (FreeList[M_Val].nodesize) << 1;
    FreeList[M_Val + 1].head = NULL;
  }
  M_Val--;
}

// This function is to find the maximum power-of-two node position that can be allocated from the remaining space
int ResidueSizeInsertHead(size_t residue_size) {
  int i;
  for (i = M_Val; i >= 0; i -= 1) {
    if (residue_size >= FreeList[i].nodesize)
      return i;
  }
  return -1;
}

int min_init(void *start, unsigned long size) // Memory space initialization
{
  PBuddy head = start;
  size_t residue_size;
  int Val;
  MallocAddress = start; // Save the starting address of memory
  residue_size = size;   // Record the size of the space
  InitFreeList(size);    // Initialize the free list
  Val = M_Val;
  while ((Val = ResidueSizeInsertHead(residue_size)) >= 0)
  // Split the memory into the largest possible power-of-two free blocks and insert them into the corresponding free lists.
  {
    head->kval = Val;
    InsertHead(head);
    head = (PBuddy)((int)head + FreeList[Val].nodesize);
    residue_size -= FreeList[Val].nodesize;
  }
  return __used;
}

void *min_malloc(size_t size) // Memory allocation
{
  PBuddy pa, pi;
  int k, i;

  for (k = 0; k <= M_Val && (FreeList[k].nodesize < size + sizeof(Buddy) ||
                             !FreeList[k].head);
       k++)
    ; // Find the memory that can be allocated, which must satisfy that the free block is larger than the user's required size plus the space occupied by a node
  if (k <= M_Val) {
    pa = DeleteHead(k); // Delete the required node
    for (
        i = 1; (k - i) >= 0 && FreeList[k - i].nodesize >= size + sizeof(Buddy);
        i +=
        1) { // Split the node into two smaller blocks until the user's required memory block size is reached.
      pi = (PBuddy)((char *)pa + FreeList[k - i].nodesize);
      pi->kval = k - i;
      InsertHead(pi);
    }
    pa->flag = __used; // Mark the memory as used
    pa->kval = k - (--i);
    return pa + 1; // Return the address after the node to the user.
  }
  return NULL; // Allocation failed, no free space available.
}

void min_free(void *p) // Memory release
{
  PBuddy pbuddy = (PBuddy)p - 1, pfriend;

  if (p == NULL) // If the user releases incorrectly
    return;

  pbuddy->flag = __unused; // Mark the space as free

  if (pbuddy->kval >= M_Val) // Case where there is no buddy block
  {
    InsertHead(pbuddy); // Insert directly at the head node.
    return;
  }
  // Two conditions must be met to merge with the buddy.
  // The buddy is free
  // The buddy exists (the buddy may have been split into smaller blocks, so when the buddy's kval is different, it means the buddy cannot be used)
  if (0 == ((((int)pbuddy) - (int)MallocAddress) %
            FreeList[pbuddy->kval + 1].nodesize)) {

    pfriend = (PBuddy)(((int)pbuddy) + FreeList[pbuddy->kval].nodesize);

    if (pfriend->kval == pbuddy->kval && pfriend->flag == __unused) {
      DeleteNode(pfriend);
      pbuddy->kval++;
      min_free(pbuddy + 1);
      return;
    }
  } else if (FreeList[pbuddy->kval].nodesize ==
             ((((int)pbuddy) - (int)MallocAddress) %
              FreeList[pbuddy->kval + 1].nodesize)) {
    pfriend = (PBuddy)(((int)pbuddy) - FreeList[pbuddy->kval].nodesize);

    if (pfriend->kval == pbuddy->kval && pfriend->flag == __unused) {
      DeleteNode(pfriend);
      pfriend->kval++;
      min_free(pfriend + 1);
      return;
    }
  }
  InsertHead(pbuddy);
}

void min_uninit() {
  if (FreeList[M_Val].head->flag == 0) {
    printf("min_free done well!\n");
  } else {
    printf("some min_malloc are not been freed!\n");
  }
}
