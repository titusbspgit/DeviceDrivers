CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Output_drive_control_OUT_SET_CLR

test_Output_drive_control_OUT_SET_CLR: tests/test_Output_drive_control_OUT_SET_CLR.c src/Output_drive_control_OUT_SET_CLR_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Output_drive_control_OUT_SET_CLR.c src/Output_drive_control_OUT_SET_CLR_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Output_drive_control_OUT_SET_CLR *.o
