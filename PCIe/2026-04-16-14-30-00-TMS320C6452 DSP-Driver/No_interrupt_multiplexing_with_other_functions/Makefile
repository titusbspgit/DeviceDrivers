CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_No_interrupt_multiplexing_with_other_functions

test_No_interrupt_multiplexing_with_other_functions: tests/test_No_interrupt_multiplexing_with_other_functions.c src/No_interrupt_multiplexing_with_other_functions_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_No_interrupt_multiplexing_with_other_functions.c src/No_interrupt_multiplexing_with_other_functions_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_No_interrupt_multiplexing_with_other_functions *.o
