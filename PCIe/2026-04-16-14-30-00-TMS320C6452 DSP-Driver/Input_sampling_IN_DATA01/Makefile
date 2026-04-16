CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Input_sampling_IN_DATA01

test_Input_sampling_IN_DATA01: tests/test_Input_sampling_IN_DATA01.c src/Input_sampling_IN_DATA01_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Input_sampling_IN_DATA01.c src/Input_sampling_IN_DATA01_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Input_sampling_IN_DATA01 *.o
