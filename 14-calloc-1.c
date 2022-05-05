#if 0
    source "${TEST_LIB}/crunner" -lallocator
#endif

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <allocator.h>

#define NUM_ROUNDS 20
#define NUM_ALLOCATIONS 100
#define ALLOC_SZ 4096

unsigned long vm_start = 0;
unsigned long vm_limit = 0;

test_start("Makes a large amount of random allocations and frees them");

subtest("Virtual Memory Size Check",
{
    srand(time(NULL));
    int i, j;
    int *allocs[NUM_ALLOCATIONS] = { NULL };

    for (i = 0; i < NUM_ROUNDS; ++i) {
        for (j = 0; j < NUM_ALLOCATIONS; ++j) {
            if (allocs[j] != 0) {
                /* There is already an allocation here */
                continue; 
            }
            unsigned int alloc_sz = rand() % ALLOC_SZ;
            int *a = calloc_impl(alloc_sz, sizeof(int), "");
            if (a == NULL) {
                test_assert(a != NULL);
                return 1;
            }

            /* Check that all values == 0. We don't use assertions here to avoid
             * spamming the console output. */
            for (int m = 0; m < alloc_sz; ++m) {
                if (a[m] != 0) {
                    printf("Value was nonzero at index %d!\n", m);
                    printf("calloc() is not fully zeroing the memory out\n");
                    return 1;
                }
            }

            /* Store junk here now that we've confirmed it's empty. If the block
             * gets reused later, this shouldn't matter. */
            memset(a, 0xFF, alloc_sz * sizeof(int));
            allocs[j] = a;
        }

        /* Free some of the allocations */
        for (j = 0; j < rand() % NUM_ALLOCATIONS + 1; ++j) {
            if (allocs[j] != NULL) {
                free_impl(allocs[j]);
                allocs[j] = NULL;
            }
        }
    }
});

test_end
