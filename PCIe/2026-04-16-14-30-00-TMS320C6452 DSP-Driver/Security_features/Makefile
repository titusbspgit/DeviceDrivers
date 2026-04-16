CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Security_features

test_Security_features: tests/test_Security_features.c src/Security_features_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Security_features.c src/Security_features_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Security_features *.o
