/*
 * Test Application — PCIe 6.0 64.0 GT/s Flit Mode (Synopsys DWC PCIe DM v6.00a)
 *
 * This is a self-contained test that uses a mock DBI memory to validate:
 *  - Capability discovery
 *  - Register programming sequence
 *  - Status polling and timeout handling
 */

#include "dw_pcie6_flit.h"
#include <stdio.h>
#include <string.h>

/* Simple delay stub for portability */
static void test_udelay(uint32_t usec)
{
    volatile uint32_t i;
    for (i = 0u; i < (usec * 10u); ++i) {
        __asm__ __volatile__("" ::: "memory");
    }
}

/* Mock DBI memory (4KB) */
static uint8_t dbi_mem[4096u];

/* Helpers to write 8/16/32 into mock DBI (reusing driver helpers would require exposure) */
static inline uint32_t w_read32(uint32_t off)
{
    uint32_t *p = (uint32_t *)&dbi_mem[off & ~0x3u];
    return *p;
}
static inline void w_write32(uint32_t off, uint32_t v)
{
    uint32_t *p = (uint32_t *)&dbi_mem[off & ~0x3u];
    *p = v;
}
static inline uint16_t w_read16(uint32_t off)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x2u) * 8u;
    uint32_t v = w_read32(aligned);
    return (uint16_t)((v >> shift) & 0xFFFFu);
}
static inline void w_write16(uint32_t off, uint16_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x2u) * 8u;
    uint32_t mask = 0xFFFFu << shift;
    uint32_t v = w_read32(aligned);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    w_write32(aligned, v);
}
static inline void w_write8(uint32_t off, uint8_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x3u) * 8u;
    uint32_t mask = 0xFFu << shift;
    uint32_t v = w_read32(aligned);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    w_write32(aligned, v);
}

/* Initialize mock capability structures */
static void init_mock_caps(void)
{
    memset(dbi_mem, 0, sizeof(dbi_mem));

    /* Capability Pointer at 0x34 -> 0x50 */
    w_write8(PCI_CFG_CAP_PTR_OFF, 0x50u);

    /* PCIe Capability at 0x50: Cap ID 0x10, Next = 0 */
    w_write8(0x50u + 0u, (uint8_t)PCIE_CAP_ID);
    w_write8(0x50u + 1u, 0x00u);

    /* Device Capabilities 3: FLIT Mode Supported = 1 */
    w_write32(0x50u + PCIE_CAP_DEVCAP3_OFF, (uint32_t)PCIE_DEVCAP3_FLIT_MODE_SUPP);

    /* Link Status: set DLLLA=1 and Current Link Speed = Gen6 to simulate success */
    uint16_t lnksta = (uint16_t)(PCIE_LNKSTA_DLLLA | PCIE_LINK_SPEED_GEN6);
    w_write16(0x50u + PCIE_CAP_LNKCTLSTS_OFF + 2u, lnksta);

    /* Device Status 3: FLIT Mode Status = 1 (simulate already in FLIT) */
    uint16_t devsts3 = (uint16_t)PCIE_DEVSTS3_FLIT_MODE_STATUS;
    uint32_t devctl3sts3 = (uint32_t)devsts3 << 16;
    w_write32(0x50u + PCIE_CAP_DEVCTL3STS3_OFF, devctl3sts3);

    /* PL64G Extended Capability at 0x100: ID=0x0031, NEXT=0 */
    uint32_t pl64g_hdr = ((uint32_t)PCIE_EXT_CAP_ID_PL64G & 0xFFFFu);
    w_write32(0x100u, pl64g_hdr);
}

static void print_status(dw_pcie6_t *ctx, const char *tag)
{
    bool flit = false;
    uint8_t sp = 0u;
    bool dll = false;
    (void)dw_pcie6_get_status(ctx, &flit, &sp, &dll);
    printf("[%s] FLIT=%u, SpeedCode=%u, DLLLA=%u\n", tag, (unsigned)flit, (unsigned)sp, (unsigned)dll);
}

int main(void)
{
    printf("DW PCIe6 Flit Mode Test Start\n");

    init_mock_caps();

    dw_pcie6_t ctx;
    int rc = dw_pcie6_init(&ctx, (uintptr_t)&dbi_mem[0]);
    if (rc != DW_PCIE6_OK) {
        printf("init failed rc=%d\n", rc);
        return 1;
    }

    dw_pcie6_set_udelay(&ctx, test_udelay);

    rc = dw_pcie6_find_caps(&ctx);
    if (rc != DW_PCIE6_OK) {
        printf("cap discovery failed rc=%d\n", rc);
        return 1;
    }
    printf("PCIE cap @0x%X, PL64G cap @0x%X\n", (unsigned)ctx.pcie_cap_off, (unsigned)ctx.pl64g_cap_off);

    if (!dw_pcie6_is_flit_supported(&ctx)) {
        printf("FLIT not supported (unexpected in mock)\n");
        return 1;
    }

    print_status(&ctx, "before");

    /* Clear DBI_RO_WR_EN to validate driver toggles it */
    w_write32(PL_MISC_CONTROL_1_OFF, 0u);

    rc = dw_pcie6_enable_flit_64g(&ctx, 1000u);
    printf("enable_flit_64g rc=%d\n", rc);

    /* Validate programming: LC2 target speed = 6, DEVCTL3 Enter FLIT set, MISC_CONTROL1 cleared */
    uint16_t lnkctl2 = w_read16(ctx.pcie_cap_off + PCIE_CAP_LNKCTL2STS2_OFF);
    uint16_t devctl3 = w_read16(ctx.pcie_cap_off + PCIE_CAP_DEVCTL3STS3_OFF);
    uint32_t misc = w_read32(PL_MISC_CONTROL_1_OFF);

    printf("Checks: LC2.TLS=%u, DEVCTL3.ENTER_FLIT=%u, MISC.DBI_RO_WR_EN=%u\n",
           (unsigned)(lnkctl2 & PCIE_LNKCTL2_TLS_MASK),
           (unsigned)((devctl3 & PCIE_DEVCTL3_ENTER_FLIT_MODE) != 0u),
           (unsigned)((misc & PL_MISC_CONTROL_1_DBI_RO_WR_EN) != 0u));

    print_status(&ctx, "after");

    /* Negative test: timeout (make status fail, tiny timeout) */
    /* Clear FLIT status and DLLLA */
    uint32_t devctl3sts3 = w_read32(ctx.pcie_cap_off + PCIE_CAP_DEVCTL3STS3_OFF) & 0x0000FFFFu;
    w_write32(ctx.pcie_cap_off + PCIE_CAP_DEVCTL3STS3_OFF, devctl3sts3); /* DS3=0 */
    w_write16(ctx.pcie_cap_off + PCIE_CAP_LNKCTLSTS_OFF + 2u, 0u); /* LNKSTA=0 */

    rc = dw_pcie6_enable_flit_64g(&ctx, 10u); /* expect timeout */
    printf("timeout test rc=%d (expected %d)\n", rc, DW_PCIE6_ERR_TIMEOUT);

    /* Negative test: remove PCIe Capability and expect CAP_NOT_FOUND */
    w_write8(PCI_CFG_CAP_PTR_OFF, 0u);
    ctx.pcie_cap_off = 0u;
    rc = dw_pcie6_find_caps(&ctx);
    printf("find_caps after removal rc=%d (expected %d)\n", rc, DW_PCIE6_ERR_CAP_NOT_FOUND);

    printf("DW PCIe6 Flit Mode Test End\n");
    return 0;
}
