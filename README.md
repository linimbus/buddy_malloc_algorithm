# buddy_malloc_algorithm

## Description

The `buddy_malloc_algorithm` is a memory allocation algorithm that efficiently manages memory blocks by dividing them into smaller chunks using a "buddy system." This system ensures that memory is allocated and deallocated in a way that minimizes fragmentation and optimizes performance. The algorithm is particularly useful in scenarios where memory blocks of varying sizes need to be managed dynamically.

## Compilation

To compile the `buddy_malloc_algorithm`, follow these steps:

```bash
git clone https://github.com/linimbus/buddy_malloc_algorithm.git
cd buddy_malloc_algorithm
make
./prog3 < prog3.input1
./prog3 < prog3.input2
```