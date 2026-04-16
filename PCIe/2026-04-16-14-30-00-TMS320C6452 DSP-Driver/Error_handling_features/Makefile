CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Error_handling_features

test_Error_handling_features: tests/test_Error_handling_features.c src/Error_handling_features_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Error_handling_features.c src/Error_handling_features_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Error_handling_features *.o
