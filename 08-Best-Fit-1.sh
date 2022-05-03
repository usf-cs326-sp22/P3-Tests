source "${TEST_LIB}/funcs.bash"

expected_order=$(cat <<EOM
[REGION ]
  [BLOCK ] 576  [USED]  'Test Allocation: 0'
  [BLOCK ] 176  [USED]  'Test Allocation: 6'
  [BLOCK ] 112  [USED]  'Test Allocation: 7'
  [BLOCK ] 80   [USED]  'Test Allocation: 8'
  [BLOCK ] 320  [USED]  'Test Allocation: 2'
  [BLOCK ] 672  [USED]  'Test Allocation: 5'
  [BLOCK ] 144  [USED]  'Test Allocation: 9'
  [BLOCK ] 256  [FREE]  ''
  [BLOCK ] 464  [USED]  'Test Allocation: 4'
  [BLOCK ] 1296 [USED]  'Test Allocation: 11'
[REGION ]
  [BLOCK ] 6736 [USED]  'Test Allocation: 10'
  [BLOCK ] 1456 [FREE]  ''
EOM
)

algo="best_fit"

test_start "Testing ${algo}"

output=$( \
    ALLOCATOR_ALGORITHM=${algo} \
    tests/progs/allocations-3)

echo "${output}"

# Get the block ordering from the output. We ignore memory addresses.
block_order=$(grep 'REGION\|BLOCK' <<< "${output}" \
    | sed 's:0x\([a-zA-Z0-9\-]*\)::g')

compare --ignore-all-space \
    <(echo "${expected_order}") <(echo "${block_order}") || test_end

test_end
