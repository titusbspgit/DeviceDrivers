CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_GPIO_banked_architecture_32_signals_2_banks

test_GPIO_banked_architecture_32_signals_2_banks: tests/test_GPIO_banked_architecture_32_signals_2_banks.c src/GPIO_banked_architecture_32_signals_2_banks_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_GPIO_banked_architecture_32_signals_2_banks.c src/GPIO_banked_architecture_32_signals_2_banks_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_GPIO_banked_architecture_32_signals_2_banks *.o
