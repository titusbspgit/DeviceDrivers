CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Initialization_sequence

test_Initialization_sequence: tests/test_Initialization_sequence.c src/Initialization_sequence_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Initialization_sequence.c src/Initialization_sequence_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Initialization_sequence *.o
