# Minimal Makefile for building GPIO drivers as static library
# Environment: Baremetal, compiler: gcc, endianness: bigendian

CC ?= gcc
AR ?= ar
CFLAGS ?= -O2 -Wall -Wextra -std=c11

SRCS := drivers/gpio/gpio_basic.c drivers/gpio/gpio_irq.c
OBJS := $(SRCS:.c=.o)
INCLUDES := -Iinclude -Idrivers/gpio -Iconfig

all: libgpio.a

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libgpio.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -f $(OBJS) libgpio.a
