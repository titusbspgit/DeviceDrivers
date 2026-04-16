CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Endianness_independence

test_Endianness_independence: tests/test_Endianness_independence.c src/Endianness_independence_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Endianness_independence.c src/Endianness_independence_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Endianness_independence *.o
