# Top-level Makefile for DWHSynopsis PCIe feature drivers and tests
# ARM big-endian target and host unit tests

TOOLCHAIN ?= arm-none-eabi-
CC       ?= $(TOOLCHAIN)gcc
AR       ?= $(TOOLCHAIN)ar
CSTD     ?= c11
CWARN    ?= -Wall -Wextra -Werror
CFLAGS_COMMON := -std=$(CSTD) $(CWARN) -ffreestanding -fno-builtin -O2
CFLAGS_HOST   := -DPCIE_HAL_HOST_TEST=1 -g
CFLAGS_TGT    := -mbig-endian
INCLUDES := -I.

SRC_COMMON := \
 drivers/pcie/common/pcie_hal.c \
 drivers/pcie/features/flit_mode/pcie_flit_mode.c \
 drivers/pcie/features/pipe_phy/pcie_pipe_phy.c \
 drivers/pcie/features/spcie/pcie_spcie.c \
 drivers/pcie/features/ltssm_eq/pcie_ltssm_eq.c \
 drivers/pcie/features/aspm/pcie_aspm.c

TEST_SRCS := \
 tests/pcie/test_runner.c \
 tests/pcie/flit_mode/test_flit_mode.c \
 tests/pcie/pipe_phy/test_pipe_phy.c \
 tests/pcie/spcie/test_spcie.c \
 tests/pcie/ltssm_eq/test_ltssm_eq.c \
 tests/pcie/aspm/test_aspm.c

OBJS_HOST := $(SRC_COMMON:.c=.host.o) $(TEST_SRCS:.c=.host.o)

all: libpcie_features.a

libpcie_features.a: $(SRC_COMMON:.c=.o)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_TGT) $(INCLUDES) -c $< -o $@

%.host.o: %.c
	gcc $(CFLAGS_COMMON) $(CFLAGS_HOST) $(INCLUDES) -c $< -o $@

.PHONY: tests clean

tests: $(OBJS_HOST)
	gcc $(CWARN) -DPCIE_HAL_HOST_TEST=1 -o build/tests_runner $(OBJS_HOST)
	./build/tests_runner || true

clean:
	rm -f $(SRC_COMMON:.c=.o) $(OBJS_HOST) libpcie_features.a build/tests_runner
	mkdir -p build
