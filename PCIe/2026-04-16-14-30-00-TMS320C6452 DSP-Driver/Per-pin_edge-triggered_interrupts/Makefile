CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Per-pin_edge-triggered_interrupts

test_Per-pin_edge-triggered_interrupts: tests/test_Per-pin_edge-triggered_interrupts.c src/Per-pin_edge-triggered_interrupts_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Per-pin_edge-triggered_interrupts.c src/Per-pin_edge-triggered_interrupts_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Per-pin_edge-triggered_interrupts *.o
