CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Open-drain_interaction_readback_behavior

test_Open-drain_interaction_readback_behavior: tests/test_Open-drain_interaction_readback_behavior.c src/Open-drain_interaction_readback_behavior_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Open-drain_interaction_readback_behavior.c src/Open-drain_interaction_readback_behavior_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Open-drain_interaction_readback_behavior *.o
