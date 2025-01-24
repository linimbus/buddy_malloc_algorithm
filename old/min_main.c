#include "min_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int rand10() { return rand() % 100; }

int main() {
  int i = 233253;
  int l, k, j;
  int *a[10000];

  int cout;

  min_init(malloc(i), i); 

  displayFreeList();

  while (1) {
    for (cout = 0; cout < 999; cout++) {
      l = rand10();
      a[cout] = (int *)min_malloc(l * sizeof(int));
      if (a[cout] != NULL) {
        for (k = 0; k < l; k++) {
          a[cout][k] = rand();
        }
        if (rand() % 2 == 0) {
          j = rand() % (cout + 1);
          min_free(a[j]);
          a[j] = NULL;
        }
      }
    }

    displayFreeList();

    for (l = 0; l < cout; ++l) {
      min_free(a[l]);
      //		printf("Free Success! %d\n",l);
    }
    displayFreeList();

    min_uninit();

    getchar();
  }
  return 0;
}
