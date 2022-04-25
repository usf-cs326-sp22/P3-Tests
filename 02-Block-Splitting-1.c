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

#define ALIGNMENT 16

size_t align(size_t orig_sz, size_t alignment)
{
    size_t new_sz = (orig_sz / alignment) * alignment;
    if (orig_sz % alignment != 0) {
        new_sz += alignment;
    }
    return new_sz;
}

test_start("Tests block splitting functionality");

subtest("Splitting a non-free block",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->size = block->size | 0x01;

    struct mem_block *result = split_block(block, 1024);
    test_assert(result == NULL);

    free(block);
});

subtest("Splitting a NULL block",
{
    struct mem_block *result = split_block(NULL, 1024);
    test_assert(result == NULL);
});

subtest("Invalid split sizes",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->size = block->size & 0x01;
    block->size = 72;

    struct mem_block *result = split_block(block, 0);
    test_assert(result == NULL);

    result = split_block(block, sizeof(struct mem_block));
    test_assert(result == NULL);

    result = split_block(
            block,
            align(sizeof(struct mem_block) + 1, ALIGNMENT));
    test_assert(result == NULL);

    free(block);
});


subtest("Block is too small to be split",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->size = block->size | 0x01;

    size_t split_sz = align(sizeof(struct mem_block) + 1, ALIGNMENT);

    block->size = 0;
    struct mem_block *result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = align(sizeof(struct mem_block) + 1, ALIGNMENT);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = align(sizeof(struct mem_block) + 2, ALIGNMENT);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    free(block);
});

subtest("Block doesn't have enough free space",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->size = block->size | 0x01;

    block->size = 8000;
    size_t split_sz = align(sizeof(struct mem_block) + 7984, ALIGNMENT);
    struct mem_block *result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = 128;
    split_sz = align(sizeof(struct mem_block) + 3, ALIGNMENT);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = 136;
    split_sz = align(sizeof(struct mem_block) + 3, ALIGNMENT);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = 143;
    split_sz = align(sizeof(struct mem_block) + 3, ALIGNMENT);
    result = split_block(block, split_sz);
    test_assert(result == NULL);


    free(block);
});

subtest("Splitting splittable blocks! (Part 1)",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->next_block = (void *) 42;
    block->size = 8000;
    block->size = block->size | 0x01;

    size_t split_sz = align(sizeof(struct mem_block) + 672, ALIGNMENT);

    struct mem_block *result = split_block(block, split_sz);
    test_assert(result != NULL);

    if (result != NULL) {
        printf(">>> We have split the block. Now let's check block properties:\n\n");
        /*
         * Things to check:
         *  - Both blocks are free when finished
         *  - The linked list is updated
         *  - Sizes are correct
         *  - The addresses are correct (relative to each other)
         *
         *  NOTE: We don't check the free list here. It might be reasonable to not
         *  update the free list during a split since most of the time one of the
         *  split blocks is about to be used.
         */

        test_assert((block->size & 0x01) == 0x01);
        test_assert((result->size & 0x01) == 0x01);

        test_assert(block->next_block == result);
        test_assert(result->next_block == (void *) 42);
        test_assert(result->prev_block == block);

        printf("\nNOTE: The two sizes below include the 'free' bit:");
        test_assert(block->size == 7265);
        test_assert(result->size == 737);

        test_assert((void *) result - (void *) block == 7264);
    }

    free(block);
});

subtest("Splitting splittable blocks! (Part 2)",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->next_block = (void *) 420;
    block->size = 160;
    block->size = block->size | 0x01;

    size_t split_sz = align(sizeof(struct mem_block) + 1, ALIGNMENT);

    struct mem_block *result = split_block(block, split_sz);
    test_assert(result != NULL);

    if (result != NULL) {
        printf(">>> We have split the block. Now let's check block properties:\n\n");

        test_assert((block->size & 0x01) == 0x01);
        test_assert(block->next_block == result);

        test_assert(result->next_block == (void *) 420);
        test_assert(result->prev_block == block);
        test_assert((result->size & 0x01) == 0x01);

        printf("\nNOTE: The two sizes below include the 'free' bit:");
        test_assert(block->size == 81);
        test_assert(result->size == 81);

        test_assert((void *) result - (void *) block == 80);
    }

    free(block);
});

subtest("Splitting splittable blocks! (Part 3)",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->next_block = (void *) 12;
    block->size = 8192;
    block->size = block->size | 0x01;

    size_t split_sz = align(sizeof(struct mem_block) + 3, ALIGNMENT);

    struct mem_block *result = split_block(block, split_sz);
    test_assert(result != NULL);

    if (result != NULL) {
        printf(">>> We have split the block. Now let's check block properties:\n\n");

        test_assert((block->size & 0x01) == 0x01);
        test_assert(block->next_block == result);

        test_assert(result->next_block == (void *) 12);
        test_assert(result->prev_block == block);
        test_assert((result->size & 0x01) == 0x01);

        test_assert(block->size == 8113);
        test_assert(result->size == 81);

        test_assert((void *) result - (void *) block == 8112);
    }

    free(block);
});




test_end
