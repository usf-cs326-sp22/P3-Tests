source "${TEST_LIB}/funcs.bash"

expected_order=$(cat <<EOM
[REGION ADDR]
  [BLOCK ADDR-ADDR] 176 [USED]  'Test Allocation: 0'
  [BLOCK ADDR-ADDR] 80  [FREE]  'Test Allocation: 1'
  [BLOCK ADDR-ADDR] 176 [USED]  'Test Allocation: 2'
  [BLOCK ADDR-ADDR] 176 [FREE]  'Test Allocation: 3'
  [BLOCK ADDR-ADDR] 176 [USED]  'Test Allocation: 4'
  [BLOCK ADDR-ADDR] 3312        [FREE]  'Test Allocation: 5'
[REGION ADDR]
  [BLOCK ADDR-ADDR] 9872        [USED]  'Big Allocation'
  [BLOCK ADDR-ADDR] 2064        [USED]  ''
  [BLOCK ADDR-ADDR] 352 [FREE]  ''
[REGION ADDR]
  [BLOCK ADDR-ADDR] 31408       [USED]  'Last alloc'
  [BLOCK ADDR-ADDR] 1360        [FREE]  ''
[ADDR] -> [ADDR] -> [ADDR] -> [ADDR] -> [ADDR] -> NULL 
EOM
)

test_start "print_memory function formatting"

# This test makes allocations that span 3 regions. Requires splitting to be
# functioning properly, but does not require FSM implementations.
output=$(tests/progs/print-test)

echo "${output}"

# Get the block ordering from the output. We replace memory addresses with ADDR
# because they will always be different. If you get 'ADDR' in the output your
# addresses are formatted correctly.
#
# The free list must end with 'NULL' to be picked up by this test.
block_order=$(grep 'REGION\|BLOCK\|NULL$' <<< "${output}" \
    | sed 's:0x\([a-zA-Z0-9\]*\):ADDR:g')

compare --ignore-all-space \
    <(echo "${expected_order}") <(echo "${block_order}") || test_end

test_end
