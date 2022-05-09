lib=liballocator.so
CFLAGS += -Wall -g -pthread -Wno-unused-variable
LDFLAGS += -I../.. -L../.. -Wl,-rpath='$$ORIGIN/../..' -lallocator

header = ../../allocator.h

all_progs = allocations-1 allocations-2 allocations-3 print-test scribble thread-safety merge

all: $(all_progs)

allocations-1: allocations-1.c $(header)
allocations-2: allocations-2.c $(header)
allocations-3: allocations-3.c $(header)
print-test:    print-test.c    $(header)
scribble:      scribble.c      $(header)
thread-safety: thread-safety.c $(header)
merge:         merge.c         $(header)

clean:
	rm -f $(all_progs)

