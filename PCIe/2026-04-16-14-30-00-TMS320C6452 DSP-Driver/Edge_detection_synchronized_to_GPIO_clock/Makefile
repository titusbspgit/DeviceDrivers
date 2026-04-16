CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_Edge_detection_synchronized_to_GPIO_clock

test_Edge_detection_synchronized_to_GPIO_clock: tests/test_Edge_detection_synchronized_to_GPIO_clock.c src/Edge_detection_synchronized_to_GPIO_clock_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_Edge_detection_synchronized_to_GPIO_clock.c src/Edge_detection_synchronized_to_GPIO_clock_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_Edge_detection_synchronized_to_GPIO_clock *.o
