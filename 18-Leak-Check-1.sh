source "${TEST_LIB}/funcs.bash"

reference_output=$(cat <<EOM
-- Leak Check --
[BLOCK ADDR] 352      ''
[BLOCK ADDR] 1360     ''
[BLOCK ADDR] 80       'Test Allocation: 1'
[BLOCK ADDR] 176      'Test Allocation: 3'
[BLOCK ADDR] 3312     'Test Allocation: 5'

-- Summary --
5 blocks lost (5285 bytes)
EOM
)

test_start "Leak Check Functionality"

ALLOCATOR_LEAK_CHECK=1 run tests/progs/print-test

# Raw output:
echo "${program_output}"

# Get the block ordering from the output. We replace memory addresses with ADDR
# because they will always be different. If you get 'ADDR' in the output your
# addresses are formatted correctly.
#
# The free list must end with 'NULL' to be picked up by this test.
program_output=$(grep -A99 '^-- Leak Check --$' <<< "${program_output}" \
    | sed 's:0x\([a-zA-Z0-9\]*\):ADDR:g')

compare_outputs --ignore-all-space || test_end 1

test_end
