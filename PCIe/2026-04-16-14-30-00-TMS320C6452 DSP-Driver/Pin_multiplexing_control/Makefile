CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Pin_multiplexing_control

test_Pin_multiplexing_control: tests/test_Pin_multiplexing_control.c src/Pin_multiplexing_control_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Pin_multiplexing_control.c src/Pin_multiplexing_control_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Pin_multiplexing_control *.o
