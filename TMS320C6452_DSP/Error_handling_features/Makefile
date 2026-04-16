CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES = -I../../common/include -Iinclude
LDFLAGS ?=
SRCS = src/feature_error.c \
       tests/test_error.c
OBJS = $(SRCS:.c=.o)

all: test.elf

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) test.elf

.PHONY: all clean
