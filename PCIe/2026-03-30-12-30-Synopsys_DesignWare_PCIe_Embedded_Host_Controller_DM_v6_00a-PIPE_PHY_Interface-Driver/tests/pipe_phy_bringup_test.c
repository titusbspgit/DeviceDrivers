/*
 * Test app for DWC PCIe DM v6.00a PIPE PHY kernel shim
 * - Reads/writes sysfs attributes (refclk_hz, ssc_enable, pipe_gen)
 * - Reads debugfs caps
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static int read_file(const char *path, char *buf, size_t buflen)
{
    FILE *f = fopen(path, "rb");
    size_t n;
    if (!f) return -errno;
    n = fread(buf, 1u, buflen - 1u, f);
    fclose(f);
    if (n == 0u) return -EIO;
    buf[n] = '\0';
    return 0;
}

static int write_file(const char *path, const char *val)
{
    FILE *f = fopen(path, "wb");
    size_t n;
    if (!f) return -errno;
    n = fwrite(val, 1u, strlen(val), f);
    fclose(f);
    return (n == strlen(val)) ? 0 : -EIO;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <platform-device-name> [--set-ssc=0|1] [--set-refclk=Hz] [--set-pipe-gen=N]\n", argv[0]);
        fprintf(stderr, "Example device: synopsys,dw-pcie-dm-pipe-phy.0 or dw-pcie-dm-pipe-phy\n");
        return 1;
    }

    const char *devname = argv[1];
    char sysfs_base[512];
    char path[640];
    char buf[256];

    snprintf(sysfs_base, sizeof(sysfs_base), "/sys/bus/platform/devices/%s", devname);

    snprintf(path, sizeof(path), "%s/refclk_hz", sysfs_base);
    if (read_file(path, buf, sizeof(buf)) == 0)
        printf("refclk_hz: %s", buf);
    else
        printf("refclk_hz: (unavailable)\n");

    snprintf(path, sizeof(path), "%s/ssc_enable", sysfs_base);
    if (read_file(path, buf, sizeof(buf)) == 0)
        printf("ssc_enable: %s", buf);
    else
        printf("ssc_enable: (unavailable)\n");

    snprintf(path, sizeof(path), "%s/pipe_gen", sysfs_base);
    if (read_file(path, buf, sizeof(buf)) == 0)
        printf("pipe_gen: %s", buf);
    else
        printf("pipe_gen: (unavailable)\n");

    for (int i = 2; i < argc; ++i) {
        if (strncmp(argv[i], "--set-ssc=", 10) == 0) {
            snprintf(path, sizeof(path), "%s/ssc_enable", sysfs_base);
            if (write_file(path, &argv[i][10]) == 0)
                printf("Set ssc_enable -> %s\n", &argv[i][10]);
            else
                printf("Set ssc_enable failed (check allow_writes=1)\n");
        } else if (strncmp(argv[i], "--set-refclk=", 13) == 0) {
            snprintf(path, sizeof(path), "%s/refclk_hz", sysfs_base);
            if (write_file(path, &argv[i][13]) == 0)
                printf("Set refclk_hz -> %s\n", &argv[i][13]);
            else
                printf("Set refclk_hz failed (check allow_writes=1)\n");
        } else if (strncmp(argv[i], "--set-pipe-gen=", 15) == 0) {
            snprintf(path, sizeof(path), "%s/pipe_gen", sysfs_base);
            if (write_file(path, &argv[i][15]) == 0)
                printf("Set pipe_gen -> %s\n", &argv[i][15]);
            else
                printf("Set pipe_gen failed (check allow_writes=1)\n");
        }
    }

    /* debugfs caps */
    if (access("/sys/kernel/debug/dw_pcie_pipe/caps", R_OK) == 0) {
        if (read_file("/sys/kernel/debug/dw_pcie_pipe/caps", buf, sizeof(buf)) == 0)
            printf("caps: %s", buf);
    } else {
        printf("debugfs caps: (unavailable)\n");
    }

    return 0;
}
