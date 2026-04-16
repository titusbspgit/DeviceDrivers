CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Direction_control_DIR01

test_Direction_control_DIR01: tests/test_Direction_control_DIR01.c src/Direction_control_DIR01_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Direction_control_DIR01.c src/Direction_control_DIR01_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Direction_control_DIR01 *.o
