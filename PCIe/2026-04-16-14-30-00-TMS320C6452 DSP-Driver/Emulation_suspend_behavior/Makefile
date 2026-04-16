CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Emulation_suspend_behavior

test_Emulation_suspend_behavior: tests/test_Emulation_suspend_behavior.c src/Emulation_suspend_behavior_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Emulation_suspend_behavior.c src/Emulation_suspend_behavior_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Emulation_suspend_behavior *.o
