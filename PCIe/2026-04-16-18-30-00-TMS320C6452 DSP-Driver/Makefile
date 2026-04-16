# SPDX-License-Identifier: MIT
# TMS320C6452 DSP GPIO driver build (host tests)

CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Werror -I ./PCIe/2026-04-16-18-30-00-TMS320C6452\ DSP-Driver/drivers/tms320c6452/hal -I ./PCIe/2026-04-16-18-30-00-TMS320C6452\ DSP-Driver/drivers/tms320c6452/gpio
LDFLAGS ?=

ROOT := PCIe/2026-04-16-18-30-00-TMS320C6452 DSP-Driver

TESTS_DIR := $(ROOT)/tests/tms320c6452
DRV_HAL := $(ROOT)/drivers/tms320c6452/hal/reg_access.c
DRV_GPIO := $(ROOT)/drivers/tms320c6452/gpio/gpio.c

BIN_DIR := $(ROOT)/build

.PHONY: all tests clean

all: tests

$(BIN_DIR):
	@mkdir -p "$(BIN_DIR)"

TEST_OBJS := $(DRV_HAL) $(DRV_GPIO) $(TESTS_DIR)/gpio_test.c

tests: $(BIN_DIR)
	$(CC) $(CFLAGS) -DRCI_MMIO_MOCK -o "$(BIN_DIR)/gpio_test" $(TEST_OBJS)

clean:
	rm -rf "$(BIN_DIR)"
