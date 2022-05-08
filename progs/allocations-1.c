#include <stdlib.h>

#include "allocator.h"

int main(void)
{
    void *a = malloc_impl(100,  "Test Allocation: 0");
    void *b = malloc_impl(10,   "Test Allocation: 1"); /* Will be deleted */
    void *c = malloc_impl(100,  "Test Allocation: 2");
    void *d = malloc_impl(100,  "Test Allocation: 3"); /* Will be deleted */
    void *e = malloc_impl(100,  "Test Allocation: 4");
    void *f = malloc_impl(3100, "Test Allocation: 5");

    free_impl(b);
    free_impl(d);

    /* This will split:
     * - f with first fit
     * - b with best fit
     * - d with worst fit
     */
    void *g = malloc_impl(10, "Test Allocation: 6");

    print_memory();

    return 0;
}
