# Makefile for TMS320C6452 GPIO driver test (bare-metal)
# Assumes GCC toolchain configured for target; big-endian build

CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -O2 -mbig-endian -ffreestanding -fno-builtin
LDFLAGS ?=
INCLUDES := -Iinclude

SRCS := src/gpio_tms320c6452.c tests/test_gpio_tms320c6452.c
OBJS := $(SRCS:.c=.o)
TARGET := gpio_test.elf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
