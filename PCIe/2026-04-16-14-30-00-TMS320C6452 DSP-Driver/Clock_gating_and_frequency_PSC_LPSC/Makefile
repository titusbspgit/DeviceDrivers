CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Clock_gating_and_frequency_PSC_LPSC

test_Clock_gating_and_frequency_PSC_LPSC: tests/test_Clock_gating_and_frequency_PSC_LPSC.c src/Clock_gating_and_frequency_PSC_LPSC_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Clock_gating_and_frequency_PSC_LPSC.c src/Clock_gating_and_frequency_PSC_LPSC_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Clock_gating_and_frequency_PSC_LPSC *.o
