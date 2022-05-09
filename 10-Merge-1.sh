source "${TEST_LIB}/funcs.bash"

reference_output=$(cat <<EOM
-- Current Memory State --

-- Free List --
NULL


-- Current Memory State --

-- Free List --
NULL
EOM
)

test_start "Merge Operation"

run tests/progs/merge

# This test case does several allocations (see merge.c), frees them, and prints
# the current memory state, which will be blank.
# Then the process repeats for a second round of allocations. Both times,
# print_memory should produce blank output (i.e., just its headings)

compare_outputs --ignore-all-space || test_end 1

test_end
