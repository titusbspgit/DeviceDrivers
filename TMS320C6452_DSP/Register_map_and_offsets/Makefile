CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -O2 -ffreestanding -fno-builtin -mbig-endian
INCLUDES = -Iinclude
LDFLAGS ?=
SRCS = src/feature_regmap.c \
       tests/test_regmap.c
OBJS = $(SRCS:.c=.o)

all: test.elf

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) test.elf

.PHONY: all clean
