CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Controller_base_address

test_Controller_base_address: tests/test_Controller_base_address.c src/Controller_base_address_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Controller_base_address.c src/Controller_base_address_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Controller_base_address *.o
