source "${TEST_LIB}/funcs.bash"

expected_order=$(cat <<EOM
[REGION ]
  [BLOCK ] 176  [USED]  'Test Allocation: 0'
  [BLOCK ] 80   [USED]  'Test Allocation: 6'
  [BLOCK ] 176  [USED]  'Test Allocation: 2'
  [BLOCK ] 176  [FREE]  'Test Allocation: 3'
  [BLOCK ] 176  [USED]  'Test Allocation: 4'
  [BLOCK ] 3168 [USED]  'Test Allocation: 5'
  [BLOCK ] 144  [FREE]  ''
EOM
)

test_start "Basic Best Fit"

output=$( \
    ALLOCATOR_ALGORITHM=best_fit \
    tests/progs/allocations-1)

echo "${output}"

# Get the block ordering from the output. We ignore memory addresses.
block_order=$(grep 'REGION\|BLOCK' <<< "${output}" \
    | sed 's:0x\([a-zA-Z0-9\-]*\)::g')

compare --ignore-all-space \
    <(echo "${expected_order}") <(echo "${block_order}") || test_end

test_end

