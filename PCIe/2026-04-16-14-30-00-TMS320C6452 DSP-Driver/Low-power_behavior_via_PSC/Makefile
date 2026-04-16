CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Low-power_behavior_via_PSC

test_Low-power_behavior_via_PSC: tests/test_Low-power_behavior_via_PSC.c src/Low-power_behavior_via_PSC_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Low-power_behavior_via_PSC.c src/Low-power_behavior_via_PSC_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Low-power_behavior_via_PSC *.o
