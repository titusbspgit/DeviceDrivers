CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Register_map_and_offsets

test_Register_map_and_offsets: tests/test_Register_map_and_offsets.c src/Register_map_and_offsets_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Register_map_and_offsets.c src/Register_map_and_offsets_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Register_map_and_offsets *.o
