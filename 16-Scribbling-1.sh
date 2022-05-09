source "${TEST_LIB}/funcs.bash"

reference_output=$(cat <<EOM
Printing uninitialized variables:
-1431655766
-1431655766
-1431655766
12297829382473034410
aaaaaaaa
aaaaaaaaaaaaaaaa
Totalling up uninitialized arrays:
850000
calloc should still zero out the memory:
0
EOM
)

test_start "Memory Scribbling"

ALLOCATOR_SCRIBBLE=1 run tests/progs/scribble

compare_outputs --ignore-all-space || test_end 1

test_end
