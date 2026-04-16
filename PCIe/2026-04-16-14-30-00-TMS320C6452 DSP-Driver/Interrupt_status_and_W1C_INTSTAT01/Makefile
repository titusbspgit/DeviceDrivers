CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Interrupt_status_and_W1C_INTSTAT01

test_Interrupt_status_and_W1C_INTSTAT01: tests/test_Interrupt_status_and_W1C_INTSTAT01.c src/Interrupt_status_and_W1C_INTSTAT01_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Interrupt_status_and_W1C_INTSTAT01.c src/Interrupt_status_and_W1C_INTSTAT01_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Interrupt_status_and_W1C_INTSTAT01 *.o
