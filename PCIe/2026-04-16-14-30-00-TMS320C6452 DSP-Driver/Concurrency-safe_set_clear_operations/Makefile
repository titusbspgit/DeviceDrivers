CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Concurrency-safe_set_clear_operations

test_Concurrency-safe_set_clear_operations: tests/test_Concurrency-safe_set_clear_operations.c src/Concurrency-safe_set_clear_operations_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Concurrency-safe_set_clear_operations.c src/Concurrency-safe_set_clear_operations_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Concurrency-safe_set_clear_operations *.o
