source "${TEST_LIB}/funcs.bash"

run_timeout=20

test_start "Thread Safety" \
    "Performs allocations across multiple threads in parallel"

# Check to make sure the library exists
[[ -e "./allocator.so" ]] || test_end 1

# If this crashes or times out, your allocator is not thread safe!
# Tip: if it takes a long time to run, try disabling LOG messages.
LD_PRELOAD=./allocator.so run tests/progs/thread-safety

test_end
