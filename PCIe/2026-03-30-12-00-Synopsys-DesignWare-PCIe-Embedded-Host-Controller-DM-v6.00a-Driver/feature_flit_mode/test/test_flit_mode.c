/* SPDX-License-Identifier: BSD-3-Clause */
/* Test for Synopsys DWC PCIe DM v6.00a — PCIe 6.0 Flit Mode */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/dw_pcie_flit.h"

/* Static mock ECAM/DBI regions to satisfy no-dynamic-memory constraint */
static volatile uint8_t mock_ecam[4096u];
static volatile uint8_t mock_dbi[4096u];

/* Helpers to synthesize an Extended Capability at offset with given ID and next */
static void write_extcap_hdr(uint32_t off, uint16_t id, uint32_t next)
{
    uint32_t hdr = 0u;
    hdr |= (uint32_t)id;
    hdr |= ((next & 0xFFFu) << 20);
    dw_mmio_write32(mock_ecam, off, hdr);
}

int main(void)
{
    printf("[FLIT TEST] Start\n");

    /* Clear mock spaces */
    memset((void *)mock_ecam, 0, sizeof(mock_ecam));
    memset((void *)mock_dbi, 0, sizeof(mock_dbi));

    /* Layout: PCIe Cap base at 0x40 (example), ExtCaps from 0x100 */
    const uint32_t pcie_cap_off = 0x40u;

    /* Create DEV3 ExtCap at 0x120 and PL64G at 0x140 */
    write_extcap_hdr(0x100u, 0xFFFFu, 0x120u); /* Dummy first cap to link list */
    write_extcap_hdr(0x120u, (uint16_t)DW_PCIE_FLIT_DEV3_EXT_CAP_ID, 0x140u);
    write_extcap_hdr(0x140u, (uint16_t)DW_PCIE_FLIT_PL64G_EXT_CAP_ID, 0x000u);

    /* Mark Flit supported in DEV3 Cap */
    dw_mmio_write32(mock_ecam, 0x120u + DW_PCIE_DEV3_CAP_OFF, DW_PCIE_DEV3_CAP_FLIT_SUPPORT);

    /* Initialize Control/Status3 register image at PCIe Cap + 0x38 */
    dw_mmio_write32(mock_ecam, pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF, 0u);

    dw_pcie_flit_ctx_t ctx;
    if (dw_pcie_flit_init(&ctx, (void *)mock_ecam, (void *)mock_dbi, pcie_cap_off) != 0)
    {
        printf("[FLIT TEST] init failed\n");
        return 1;
    }
    (void)dw_pcie_flit_discover(&ctx);

    if (!dw_pcie_flit_supported(&ctx))
    {
        printf("[FLIT TEST] flit not supported — SKIP\n");
        return 0;
    }

    /* Enable: simulate hardware asserting FLIT_ACTIVE after we request */
    int32_t rc = dw_pcie_flit_enable(&ctx, 10u);
    if (rc != -3) /* we expect timeout unless we set status */
    {
        printf("[FLIT TEST] unexpected rc=%ld before status set\n", (long)rc);
    }

    /* Now set FLIT_ACTIVE and retry */
    uint32_t v = dw_mmio_read32(mock_ecam, pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
    v |= DW_PCIE_LS3_FLIT_ACTIVE;
    dw_mmio_write32(mock_ecam, pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF, v);

    rc = dw_pcie_flit_enable(&ctx, 10u);
    if (rc != 0)
    {
        printf("[FLIT TEST] enable failed rc=%ld\n", (long)rc);
        return 1;
    }

    if (!dw_pcie_flit_active(&ctx))
    {
        printf("[FLIT TEST] active check failed\n");
        return 1;
    }

    /* Disable: simulate hardware clearing FLIT_ACTIVE */
    v = dw_mmio_read32(mock_ecam, pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
    v &= ~DW_PCIE_LS3_FLIT_ACTIVE;
    dw_mmio_write32(mock_ecam, pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF, v);

    rc = dw_pcie_flit_disable(&ctx, 10u);
    if (rc != 0)
    {
        printf("[FLIT TEST] disable failed rc=%ld\n", (long)rc);
        return 1;
    }

    printf("[FLIT TEST] PASS\n");
    return 0;
}
