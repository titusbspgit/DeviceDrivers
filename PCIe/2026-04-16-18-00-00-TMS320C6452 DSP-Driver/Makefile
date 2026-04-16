# SPDX-License-Identifier: MIT
# Simple build for drivers and tests (host GCC for tests with MMIO mock)

CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Werror -O2
INCS = -IPCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/drivers/tms320c6452

DRIVERS = \
 PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/drivers/tms320c6452/hal/reg_access.c \
 PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/drivers/tms320c6452/gpio/gpio.c

TESTS = \
 PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/tests/tms320c6452/gpio_test.c

all: tests

.PHONY: drivers tests clean

drivers:
	@echo "Drivers are header-only plus minimal C; no standalone build target."

PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/build:
	@mkdir -p $@

# Build test with MMIO mock
PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/build/gpio_test: $(DRIVERS) $(TESTS)
	$(CC) $(CFLAGS) -DRCI_MMIO_MOCK $(INCS) -o $@ $(DRIVERS) $(TESTS)

tests: PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/build PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/build/gpio_test
	@echo "Built tests. Run ./PCIe/2026-04-16-18-00-00-TMS320C6452\\ DSP-Driver/build/gpio_test"

clean:
	rm -rf PCIe/2026-04-16-18-00-00-TMS320C6452\ DSP-Driver/build
