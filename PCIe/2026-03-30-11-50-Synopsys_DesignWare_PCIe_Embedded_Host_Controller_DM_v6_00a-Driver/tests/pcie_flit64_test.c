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
#include <time.h>

static int read_cfg16(const char *bdf, unsigned off, unsigned *out)
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

static int read_lnksts_flags(const char *bdf, unsigned cap_base, unsigned *training, unsigned *dllla)
{
    unsigned v = 0u;
    if (read_cfg16(bdf, cap_base + 0x12u, &v) != 0) {
        return -1;
    }
    *training = (v >> 11) & 0x1u;
    *dllla = (v >> 13) & 0x1u;
    return 0;
}

static int read_lnksts2(const char *bdf, unsigned cap_base, unsigned *lnksts2)
{
    unsigned v = 0u;
    if (read_cfg16(bdf, cap_base + 0x30u + 2u, &v) != 0) { /* upper16 of +0x30 */
        return -1;
    }
    *lnksts2 = v;
    return 0;
}

static int read_write_devctl3(const char *bdf, unsigned pcie_base, bool write, unsigned *val)
{
    unsigned off = pcie_base + 0x38u; /* DevCtl3/DevSts3 dw; DevCtl3 is lo16 */
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

static int write_lnkctl2_tgt64(const char *bdf, unsigned pcie_base)
{
    unsigned off = pcie_base + 0x30u; /* LnkCtl2/LnkSts2 dw; LnkCtl2 is lo16 */
    unsigned v = 0u;
    if (read_cfg16(bdf, off, &v) != 0) { return -1; }
    v = (v & ~0xFu) | 0x6u;
    if (write_cfg16(bdf, off, v) != 0) { return -1; }
    return 0;
}

static int set_retrain(const char *bdf, unsigned pcie_base)
{
    unsigned off = pcie_base + 0x10u; /* Link Control */
    unsigned v = 0u;
    if (read_cfg16(bdf, off, &v) != 0) { return -1; }
    v |= 0x20u;
    if (write_cfg16(bdf, off, v) != 0) { return -1; }
    return 0;
}

static void sleep_us(unsigned us)
{
    struct timespec ts;
    ts.tv_sec = us / 1000000u;
    ts.tv_nsec = (long)((us % 1000000u) * 1000u);
    (void)nanosleep(&ts, NULL);
}

int main(int argc, char **argv)
{
    const char *bdf = (argc > 1) ? argv[1] : "00:00.0";

    printf("pcie_flit64_test: using BDF=%s\n", bdf);

    unsigned pcie_base = 0u;
    if (read_pcie_cap_base(bdf, &pcie_base) != 0) {
        fprintf(stderr, "Failed to read PCIe Cap base\n");
        return 1;
    }

    unsigned speed = 0u, training = 0u, dllla = 0u;
    if (read_lnksts_speed(bdf, pcie_base, &speed) != 0) {
        fprintf(stderr, "Failed to read link speed\n");
        return 1;
    }
    (void)read_lnksts_flags(bdf, pcie_base, &training, &dllla);

    printf("Initial: speed=0x%x (%s) training=%u dllla=%u\n",
           speed, (speed == 6u) ? "64.0 GT/s" : "<64.0 GT/s", training, dllla);

    unsigned devctl3 = 0u;
    if (read_write_devctl3(bdf, pcie_base, false, &devctl3) != 0) {
        fprintf(stderr, "Failed to read DevCtl3\n");
        return 1;
    }

    unsigned lnksts2 = 0u;
    if (read_lnksts2(bdf, pcie_base, &lnksts2) != 0) {
        fprintf(stderr, "Failed to read LnkSts2\n");
        return 1;
    }

    printf("Flit: enabled=%u active=%u (LnkSts2 bit10)\n",
           (devctl3 & 0x1u) ? 1u : 0u,
           ((lnksts2 & (1u << 10)) != 0u) ? 1u : 0u);

    if (argc > 2 && strcmp(argv[2], "--toggle-on") == 0) {
        /* Enable Flit, set 64G target, retrain, and poll */
        devctl3 |= 0x1u;
        if (read_write_devctl3(bdf, pcie_base, true, &devctl3) != 0) {
            fprintf(stderr, "Failed to write DevCtl3\n");
            return 1;
        }
        if (write_lnkctl2_tgt64(bdf, pcie_base) != 0) {
            fprintf(stderr, "Failed to program LnkCtl2\n");
            return 1;
        }
        if (set_retrain(bdf, pcie_base) != 0) {
            fprintf(stderr, "Failed to retrain\n");
            return 1;
        }

        /* Poll for training complete and DLLLA up to 2s */
        unsigned timeout = 2000000u;
        while (timeout > 0u) {
            if (read_lnksts_flags(bdf, pcie_base, &training, &dllla) != 0) {
                break;
            }
            if ((training == 0u) && (dllla == 1u)) {
                break;
            }
            sleep_us(1000u); /* 1ms */
            timeout -= 1000u;
        }
        if (timeout == 0u) {
            fprintf(stderr, "Timeout waiting for L0\n");
            return 2;
        }

        /* Verify 64G and Flit Active within another 2s */
        timeout = 2000000u;
        while (timeout > 0u) {
            if (read_lnksts_speed(bdf, pcie_base, &speed) != 0) { break; }
            if (read_lnksts2(bdf, pcie_base, &lnksts2) != 0) { break; }
            if ((speed == 6u) && ((lnksts2 & (1u << 10)) != 0u)) {
                break;
            }
            sleep_us(1000u);
            timeout -= 1000u;
        }
        if (timeout == 0u) {
            fprintf(stderr, "Flit/64G not confirmed within timeout\n");
            return 3;
        }
        printf("Post-retrain: speed=64.0 GT/s, Flit Active=1\n");
    }

    /* If PL64G cap exists, dump STATUS for diagnostics */
    /* Naive scan: walk 0x100..0xFFF and check ID 0x0031 */
    unsigned off = 0x100u;
    while (off >= 0x100u && off < 0x1000u) {
        unsigned hdr0 = 0u;
        if (read_cfg16(bdf, off + 0x0u, &hdr0) != 0) { break; }
        unsigned id = hdr0 & 0xFFFFu;
        if (id == 0x0031u) {
            unsigned stat_lo = 0u, stat_hi = 0u;
            if (read_cfg16(bdf, off + 0x0Cu, &stat_lo) == 0 &&
                read_cfg16(bdf, off + 0x0Eu, &stat_hi) == 0) {
                unsigned status_dw = stat_lo | (stat_hi << 16);
                printf("PL64G STATUS @0x%03x = 0x%08x\n", off, status_dw);
            }
            break;
        }
        unsigned hdr1 = 0u;
        if (read_cfg16(bdf, off + 0x02u, &hdr1) != 0) { break; }
        unsigned next = (hdr1 >> 4) & 0x0FFFu;
        if (next == 0u) { break; }
        off = next;
    }

    return 0;
}
