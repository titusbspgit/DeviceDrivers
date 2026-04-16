CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Per-bank_interrupt_enable_BINTEN

test_Per-bank_interrupt_enable_BINTEN: tests/test_Per-bank_interrupt_enable_BINTEN.c src/Per-bank_interrupt_enable_BINTEN_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Per-bank_interrupt_enable_BINTEN.c src/Per-bank_interrupt_enable_BINTEN_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Per-bank_interrupt_enable_BINTEN *.o
