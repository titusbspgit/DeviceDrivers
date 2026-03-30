/*
 * User-space test for Synopsys DWC PCIe DM v6.00a - PCIe 6.0 Flit Mode
 * RCI-ag-DrvGen Agent
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

/* The test assumes an integration layer provides cfg ops over an fd or sysfs.
 * For portability, we provide a Linux userspace path using setpci via system().
 * In production, replace with direct config space mmap/ioctl implementation.
 */

static int exec_cmd(const char *cmd)
{
    int rc = system(cmd);
    if (rc == -1) {
        return -1;
    }
    if (WIFEXITED(rc) && (WEXITSTATUS(rc) == 0)) {
        return 0;
    }
    return -1;
}

static int read_cfg16(const char *bdf, unsigned off, unsigned *out)
{
    char cmd[256];
    char path[256];
    snprintf(path, sizeof(path), "/sys/bus/pci/devices/0000:%s/config", bdf);
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    if (fseek(f, (long)off, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }
    unsigned char buf[2];
    size_t n = fread(buf, 1u, 2u, f);
    fclose(f);
    if (n != 2u) {
        return -1;
    }
    *out = (unsigned)buf[0] | ((unsigned)buf[1] << 8);
    return 0;
}

static int write_cfg16(const char *bdf, unsigned off, unsigned val)
{
    char path[256];
    snprintf(path, sizeof(path), "/sys/bus/pci/devices/0000:%s/config", bdf);
    FILE *f = fopen(path, "r+b");
    if (f == NULL) {
        return -1;
    }
    if (fseek(f, (long)off, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }
    unsigned char buf[2];
    buf[0] = (unsigned char)(val & 0xFFu);
    buf[1] = (unsigned char)((val >> 8) & 0xFFu);
    size_t n = fwrite(buf, 1u, 2u, f);
    fclose(f);
    return (n == 2u) ? 0 : -1;
}

static int read_cfg8(const char *bdf, unsigned off, unsigned *out)
{
    char path[256];
    snprintf(path, sizeof(path), "/sys/bus/pci/devices/0000:%s/config", bdf);
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    if (fseek(f, (long)off, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }
    unsigned char v = 0u;
    size_t n = fread(&v, 1u, 1u, f);
    fclose(f);
    if (n != 1u) {
        return -1;
    }
    *out = (unsigned)v;
    return 0;
}

static int read_pcie_cap_base(const char *bdf, unsigned *cap_base)
{
    /* PCI Capability pointer at 0x34 */
    unsigned cap_ptr = 0u;
    if (read_cfg8(bdf, 0x34u, &cap_ptr) != 0) {
        return -1;
    }
    *cap_base = cap_ptr;
    return 0;
}

static int read_lnksts_speed(const char *bdf, unsigned cap_base, unsigned *speed)
{
    unsigned v = 0u;
    if (read_cfg16(bdf, cap_base + 0x12u, &v) != 0) {
        return -1;
    }
    *speed = v & 0xFu;
    return 0;
}

static int read_write_devctl3(const char *bdf, unsigned pcie_base, bool write, unsigned *val)
{
    unsigned off = pcie_base + 0x20u; /* Assumption */
    if (!write) {
        unsigned v = 0u;
        if (read_cfg16(bdf, off, &v) != 0) { return -1; }
        *val = v;
        return 0;
    } else {
        if (write_cfg16(bdf, off, *val) != 0) { return -1; }
        return 0;
    }
}

static int read_devsts3(const char *bdf, unsigned pcie_base, unsigned *val)
{
    unsigned off = pcie_base + 0x22u; /* Assumption */
    unsigned v = 0u;
    if (read_cfg16(bdf, off, &v) != 0) { return -1; }
    *val = v;
    return 0;
}

static int write_lnkctl2_tgt64(const char *bdf, unsigned pcie_base)
{
    unsigned off = pcie_base + 0x30u; /* Assumption */
    unsigned v = 0u;
    if (read_cfg16(bdf, off, &v) != 0) { return -1; }
    v = (v & ~0xFu) | 0x6u;
    if (write_cfg16(bdf, off, v) != 0) { return -1; }
    return 0;
}

static int set_retrain(const char *bdf, unsigned pcie_base)
{
    unsigned off = pcie_base + 0x10u;
    unsigned v = 0u;
    if (read_cfg16(bdf, off, &v) != 0) { return -1; }
    v |= 0x20u;
    if (write_cfg16(bdf, off, v) != 0) { return -1; }
    return 0;
}

int main(int argc, char **argv)
{
    const char *bdf = (argc > 1) ? argv[1] : "00:00.0"; /* Default Root Port */

    printf("pcie_flit64_test: using BDF=%s\n", bdf);

    unsigned pcie_base = 0u;
    if (read_pcie_cap_base(bdf, &pcie_base) != 0) {
        fprintf(stderr, "Failed to read PCIe Cap base\n");
        return 1;
    }

    unsigned speed = 0u;
    if (read_lnksts_speed(bdf, pcie_base, &speed) != 0) {
        fprintf(stderr, "Failed to read link status\n");
        return 1;
    }
    printf("Negotiated Link Speed: 0x%x (%s)\n", speed, (speed == 6u) ? "64.0 GT/s" : "<64.0 GT/s");

    unsigned devctl3 = 0u;
    if (read_write_devctl3(bdf, pcie_base, false, &devctl3) != 0) {
        fprintf(stderr, "Failed to read Device Control 3\n");
        return 1;
    }
    unsigned devsts3 = 0u;
    if (read_devsts3(bdf, pcie_base, &devsts3) != 0) {
        fprintf(stderr, "Failed to read Device Status 3\n");
        return 1;
    }

    printf("Flit Mode: enabled=%u active=%u\n", (devctl3 & 0x1u) ? 1u : 0u, (devsts3 & 0x1u) ? 1u : 0u);

    if (argc > 2 && strcmp(argv[2], "--toggle-on") == 0) {
        /* Request enable + retrain */
        devctl3 |= 0x1u;
        if (read_write_devctl3(bdf, pcie_base, true, &devctl3) != 0) {
            fprintf(stderr, "Failed to write Device Control 3\n");
            return 1;
        }
        if (write_lnkctl2_tgt64(bdf, pcie_base) != 0) {
            fprintf(stderr, "Failed to set target 64.0 GT/s\n");
            return 1;
        }
        if (set_retrain(bdf, pcie_base) != 0) {
            fprintf(stderr, "Failed to retrain\n");
            return 1;
        }
        printf("Requested Flit Mode enable and retrain to 64.0 GT/s. Re-run to verify.\n");
    }

    return 0;
}
