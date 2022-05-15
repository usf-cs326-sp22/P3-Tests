source "${TEST_LIB}/funcs.bash"

expected_order=$(cat <<EOM
[REGION ]
  [BLOCK ] 576  [USED]  'Realloc 0'
  [BLOCK ] 688  [USED]  'Realloc 2'
  [BLOCK ] 672  [USED]  'Realloc 5'
  [BLOCK ] 400  [FREE]  ''
  [BLOCK ] 1344 [USED]  'Realloc 3'
  [BLOCK ] 416  [FREE]  ''
[REGION ]
  [BLOCK ] 9664 [USED]  'Realloc 1'
  [BLOCK ] 2624 [FREE]  ''
EOM
)

test_start "Realloc"

output=$(tests/progs/realloc)

echo "${output}"

# Get the block ordering from the output. We ignore memory addresses.
block_order=$(grep 'REGION\|BLOCK' <<< "${output}" \
    | sed 's:0x\([a-zA-Z0-9\-]*\)::g')

compare --ignore-all-space \
    <(echo "${expected_order}") <(echo "${block_order}") || test_end

test_end
