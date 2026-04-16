// SPDX-License-Identifier: MIT
// Simple unit test for TMS320C6452 GPIO driver using MMIO mock
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RCI_MMIO_MOCK 1
#include "../../drivers/tms320c6452/hal/reg_access.h"
#include "../../drivers/tms320c6452/gpio/gpio.h"

// Override platform hooks for test visibility
void tmsgpio_platform_enable_psc(void) { printf("[TEST] PSC enable called\n"); }
void tmsgpio_platform_disable_psc(void) { printf("[TEST] PSC disable called\n"); }
void tmsgpio_platform_configure_pinmux(void) { printf("[TEST] Pinmux configure called\n"); }

// Helper to compute address
static inline uintptr_t A(uintptr_t base, uint32_t off) { return base + (uintptr_t)off; }

int main(void)
{
    const uintptr_t base = (uintptr_t)TMS320C6452_GPIO_BASE_DEFAULT;
    static uint8_t mock_space[0x100];
    rci_mock_init(base, mock_space, sizeof(mock_space));

    // Emulate reset values via writes (RO in real HW; acceptable in mock)
    rci_reg_write32(A(base, GPIO_BINTEN_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_DIR01_OFFSET), 0xFFFFFFFFu);
    rci_reg_write32(A(base, GPIO_OUT_DATA01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_SET_DATA01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_CLR_DATA01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_SET_RIS_TRIG01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_CLR_RIS_TRIG01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_SET_FAL_TRIG01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_CLR_FAL_TRIG01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_INTSTAT01_OFFSET), 0x00000000u);
    rci_reg_write32(A(base, GPIO_PID_OFFSET), 0x00000483u);

    tmsgpio_t gpio;
    if (tmsgpio_init(&gpio, base, true) != TMSGPIO_OK) {
        printf("INIT FAIL\n");
        return 1;
    }

    // Verify PID
    uint32_t pid = 0u;
    (void)tmsgpio_get_pid(&gpio, &pid);
    printf("PID=0x%08X\n", pid);

    // Configure pin 5 as output and set high, then low
    (void)tmsgpio_config_pin(&gpio, 5u, TMSGPIO_DIR_OUTPUT);
    uint32_t dir = rci_reg_read32(A(base, GPIO_DIR01_OFFSET));
    printf("DIR after pin5 OUT=0x%08X\n", dir);

    (void)tmsgpio_write_pin(&gpio, 5u, true);
    uint32_t setreg = rci_reg_read32(A(base, GPIO_SET_DATA01_OFFSET));
    printf("SET reg after pin5 high=0x%08X\n", setreg);

    (void)tmsgpio_write_pin(&gpio, 5u, false);
    uint32_t clrreg = rci_reg_read32(A(base, GPIO_CLR_DATA01_OFFSET));
    printf("CLR reg after pin5 low=0x%08X\n", clrreg);

    // Read a pin level (mock IN is 0). Expect false
    bool lev = true;
    (void)tmsgpio_read_pin(&gpio, 5u, &lev);
    printf("IN pin5 level=%u\n", lev ? 1u : 0u);

    // Enable rising edge on pin 7, falling disable
    (void)tmsgpio_irq_config_edge(&gpio, 7u, true, false);
    uint32_t rise = rci_reg_read32(A(base, GPIO_SET_RIS_TRIG01_OFFSET));
    uint32_t falc = rci_reg_read32(A(base, GPIO_CLR_FAL_TRIG01_OFFSET));
    printf("RISE_EN reg=0x%08X, FAL_DIS reg=0x%08X\n", rise, falc);

    // Enable bank 0 interrupts
    (void)tmsgpio_irq_bank_enable(&gpio, 0u, true);
    uint32_t binten = rci_reg_read32(A(base, GPIO_BINTEN_OFFSET));
    printf("BINTEN=0x%08X\n", binten);

    // Simulate an interrupt on pin 7 and clear it
    rci_reg_write32(A(base, GPIO_INTSTAT01_OFFSET), 0x00000080u); // set bit7 pending in mock
    uint32_t st = 0u;
    (void)tmsgpio_irq_status(&gpio, &st);
    printf("INTSTAT before clr=0x%08X\n", st);
    (void)tmsgpio_irq_clear(&gpio, 0x00000080u);
    st = rci_reg_read32(A(base, GPIO_INTSTAT01_OFFSET));
    printf("INTSTAT after clr=0x%08X\n", st);

    // Low power enter/exit
    (void)tmsgpio_enter_low_power(&gpio);
    (void)tmsgpio_exit_low_power(&gpio);

    printf("GPIO test completed.\n");
    return 0;
}
