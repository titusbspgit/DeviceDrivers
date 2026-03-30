# Minimal Makefile for building tests (toolchain configurable)
CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -O2 -Iinclude -Idrivers/common -Idrivers/link -Idrivers/iatu -Idrivers/msi -Itests/common
LDFLAGS :=

SRCS_COMMON := drivers/common/dw_pcie_common.c tests/common/test_common.c
SRCS_LINK := $(SRCS_COMMON) drivers/link/dw_pcie_link.c tests/link_test.c
SRCS_IATU := $(SRCS_COMMON) drivers/iatu/dw_pcie_iatu.c tests/iatu_test.c
SRCS_MSI := $(SRCS_COMMON) drivers/msi/dw_pcie_msi.c tests/msi_test.c

all: link_test iatu_test msi_test

link_test: $(SRCS_LINK)
	$(CC) $(CFLAGS) -o $@ $(SRCS_LINK) $(LDFLAGS)

iatu_test: $(SRCS_IATU)
	$(CC) $(CFLAGS) -o $@ $(SRCS_IATU) $(LDFLAGS)

msi_test: $(SRCS_MSI)
	$(CC) $(CFLAGS) -o $@ $(SRCS_MSI) $(LDFLAGS)

clean:
	rm -f link_test iatu_test msi_test
