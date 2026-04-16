# Simple Makefile for building per-feature tests
# Toolchain and flags (adjust as needed)

CROSS ?= arm-none-eabi-
CC    := $(CROSS)gcc
AR    := $(CROSS)ar
OBJDUMP := $(CROSS)objdump
OBJCOPY := $(CROSS)objcopy

CFLAGS := -std=c11 -O2 -g3 -Wall -Wextra -Werror -ffreestanding -fno-builtin \
          -fno-exceptions -fno-asynchronous-unwind-tables -fno-pic -mthumb -mbig-endian
LDFLAGS := -nostdlib -Wl,--gc-sections

INCLUDES := -I./include -I./hal -I./drivers/core -I./drivers/link -I./drivers/cfg -I./drivers/msi -I./drivers/intr -I./drivers/dma

CORE_OBJS := drivers/core/pcie_core.o
LINK_OBJS := drivers/link/pcie_link.o
CFG_OBJS  := drivers/cfg/pcie_cfg.o
MSI_OBJS  := drivers/msi/pcie_msi.o
INTR_OBJS := drivers/intr/pcie_intr.o
DMA_OBJS  := drivers/dma/pcie_dma.o

COMMON_OBJS := $(CORE_OBJS)

all: test_core.elf test_link.elf test_cfg.elf test_msi.elf test_intr.elf test_dma.elf

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link each test as a standalone image (no runtime provided; platform-specific)

test_core.elf: tests/test_core.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_link.elf: tests/test_link.o $(COMMON_OBJS) $(LINK_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_cfg.elf: tests/test_cfg.o $(COMMON_OBJS) $(CFG_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_msi.elf: tests/test_msi.o $(COMMON_OBJS) $(MSI_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_intr.elf: tests/test_intr.o $(COMMON_OBJS) $(INTR_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_dma.elf: tests/test_dma.o $(COMMON_OBJS) $(DMA_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(CORE_OBJS) $(LINK_OBJS) $(CFG_OBJS) $(MSI_OBJS) $(INTR_OBJS) $(DMA_OBJS) \
	      tests/*.o *.elf

.PHONY: all clean
