#include <stdlib.h>
#include "allocator.h"

int main(void)
{
    /* Makes allocations that span 3 regions. Requires splitting to be
     * functioning properly, but does not require FSM implementations. */
    void *a = malloc_impl(100,  "Test Allocation: 0");
    void *b = malloc_impl(10,   "Test Allocation: 1"); /* Will be deleted */
    void *c = malloc_impl(100,  "Test Allocation: 2");
    void *d = malloc_impl(100,  "Test Allocation: 3"); /* Will be deleted */
    void *e = malloc_impl(100,  "Test Allocation: 4");
    void *f = malloc_impl(3232, "Test Allocation: 5"); /* Will be deleted */

    void *g = malloc_impl(9800, "Big Allocation");
    void *h = malloc_impl(2000, "");

    void *i = malloc_impl(31337, "Last alloc");

    free_impl(b);
    free_impl(d);
    free_impl(f);

    print_memory();

    return 0;
}
