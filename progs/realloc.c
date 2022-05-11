#include <stdlib.h>
#include "allocator.h"

int main(void)
{
    void *a = malloc_impl(500,  "Test Allocation: 0");
    void *b = malloc_impl(294,  "Test Allocation: 1");
    void *c = malloc_impl(250,  "Test Allocation: 2");
    void *d = malloc_impl(1000, "Test Allocation: 3");
    void *e = malloc_impl(400,  "Test Allocation: 4");

    /* Does not actually need to be resized */
    a = realloc_impl(a, 510,  "Realloc 0");

    /* No room, must free + allocate new */
    c = realloc_impl(c, 9600, "Realloc 1");

    /* Now that 'c' is free, we can expand 'b' into it */
    b = realloc_impl(b, 600,  "Realloc 2");

    /* Expand into free space at the end of the region. This will produce a new
     * free block out of the remaining space */
    e = realloc_impl(e, 1280, "Realloc 3");

    /* Shrink 'd' down to make a new free block */
    d = realloc_impl(d, 800,  "Realloc 4");

    /* Shrink 'd' one more time to make sure that the two neighboring free
     * blocks get merged back together */
    d = realloc_impl(d, 600,  "Realloc 5");

    print_memory();

    return 0;
}
