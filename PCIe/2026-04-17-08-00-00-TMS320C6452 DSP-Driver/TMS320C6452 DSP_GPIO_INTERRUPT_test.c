/* RCI-ag-DrvGen Agent: Unit test for GPIO Interrupt driver */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452 DSP_GPIO_INTERRUPT.h"

/* Stub MMIO space up to INTSTAT01 */
#define STUB_WORDS ((GPIO_INTSTAT01_OFFSET / 4u) + 1u)
static uint32_t gpio_stub_mem[STUB_WORDS];

static void reset_stub(void)
{
    memset((void *)gpio_stub_mem, 0, sizeof(gpio_stub_mem));
}

static inline void stub_write(uint32_t offset, uint32_t value) { gpio_stub_mem[offset/4u] = value; }
static inline uint32_t stub_read(uint32_t offset) { return gpio_stub_mem[offset/4u]; }

int main(void)
{
    bool pend;

    reset_stub();
    gpio_irq_init((uintptr_t)gpio_stub_mem);

    /* Configure pin 5: rising edge */
    if (gpio_irq_config(5u, GPIO_IRQ_EDGE_RISE) != GPIO_IRQ_OK) { printf("ERR: config rise\n"); return 1; }
    /* Expect SET_RIS has bit5 set, CLR_FAL has bit5 set (disabled fall) */
    if ((stub_read(GPIO_SET_RIS_TRIG01_OFFSET) & (1u<<5u)) == 0u) { printf("ERR: rise not set\n"); return 1; }
    if ((stub_read(GPIO_CLR_FAL_TRIG01_OFFSET) & (1u<<5u)) == 0u) { printf("ERR: fall not cleared\n"); return 1; }

    /* Enable bank (pin 5 in bank 0 -> EN0) */
    if (gpio_irq_enable(5u) != GPIO_IRQ_OK) { printf("ERR: enable\n"); return 1; }
    if ((stub_read(GPIO_BINTEN_OFFSET) & 0x1u) == 0u) { printf("ERR: EN0 not set\n"); return 1; }

    /* Simulate interrupt arrival by setting INTSTAT bit */
    stub_write(GPIO_INTSTAT01_OFFSET, (1u<<5u));
    if (gpio_irq_status(5u, &pend) != GPIO_IRQ_OK || !pend) { printf("ERR: status should be pending\n"); return 1; }

    /* Clear pending via W1C */
    if (gpio_irq_clear(5u) != GPIO_IRQ_OK) { printf("ERR: clear\n"); return 1; }
    /* Driver wrote W1C; emulate HW clear by zeroing bit */
    stub_write(GPIO_INTSTAT01_OFFSET, 0u);
    if (gpio_irq_status(5u, &pend) != GPIO_IRQ_OK || pend) { printf("ERR: status should be cleared\n"); return 1; }

    /* Configure both edges on pin 20 (bank1) */
    if (gpio_irq_config(20u, GPIO_IRQ_EDGE_BOTH) != GPIO_IRQ_OK) { printf("ERR: config both\n"); return 1; }
    if ((stub_read(GPIO_SET_RIS_TRIG01_OFFSET) & (1u<<20u)) == 0u) { printf("ERR: rise(20) not set\n"); return 1; }
    if ((stub_read(GPIO_SET_FAL_TRIG01_OFFSET) & (1u<<20u)) == 0u) { printf("ERR: fall(20) not set\n"); return 1; }

    /* Enable bank1 */
    if (gpio_irq_enable(20u) != GPIO_IRQ_OK) { printf("ERR: enable bank1\n"); return 1; }
    if ((stub_read(GPIO_BINTEN_OFFSET) & 0x2u) == 0u) { printf("ERR: EN1 not set\n"); return 1; }

    /* Disable bank1 and check */
    if (gpio_irq_disable(20u) != GPIO_IRQ_OK) { printf("ERR: disable bank1\n"); return 1; }
    if ((stub_read(GPIO_BINTEN_OFFSET) & 0x2u) != 0u) { printf("ERR: EN1 not cleared\n"); return 1; }

    /* Parameter validation */
    if (gpio_irq_config(32u, GPIO_IRQ_EDGE_RISE) != GPIO_IRQ_ERR_PARAM) { printf("ERR: param pin32\n"); return 1; }
    if (gpio_irq_status(1u, (bool *)0) != GPIO_IRQ_ERR_PARAM) { printf("ERR: param NULL status\n"); return 1; }

    printf("GPIO_INTERRUPT tests PASS\n");
    return 0;
}
