CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Reset_behavior_software_vs_hardware

test_Reset_behavior_software_vs_hardware: tests/test_Reset_behavior_software_vs_hardware.c src/Reset_behavior_software_vs_hardware_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Reset_behavior_software_vs_hardware.c src/Reset_behavior_software_vs_hardware_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Reset_behavior_software_vs_hardware *.o
