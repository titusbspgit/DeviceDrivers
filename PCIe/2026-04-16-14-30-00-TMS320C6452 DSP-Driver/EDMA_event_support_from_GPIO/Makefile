CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES=-I../common/include -Iinclude

all: test_EDMA_event_support_from_GPIO

test_EDMA_event_support_from_GPIO: tests/test_EDMA_event_support_from_GPIO.c src/EDMA_event_support_from_GPIO_api.c ../common/src/gpio_tms320c6452.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ tests/test_EDMA_event_support_from_GPIO.c src/EDMA_event_support_from_GPIO_api.c ../common/src/gpio_tms320c6452.c

clean:
	rm -f test_EDMA_event_support_from_GPIO *.o
