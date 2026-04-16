CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Peripheral_Identification_PID

test_Peripheral_Identification_PID: tests/test_Peripheral_Identification_PID.c src/Peripheral_Identification_PID_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Peripheral_Identification_PID.c src/Peripheral_Identification_PID_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Peripheral_Identification_PID *.o
