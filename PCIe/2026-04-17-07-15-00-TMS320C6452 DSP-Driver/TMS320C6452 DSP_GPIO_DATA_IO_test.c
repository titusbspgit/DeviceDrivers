/* RCI-ag-DrvGen Agent: Unit test for GPIO Data I/O driver */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452 DSP_GPIO_DATA_IO.h"

/* Stub memory to emulate GPIO regs up to IN_DATA01 */
#define STUB_WORDS  ((GPIO_IN_DATA01_OFFSET / 4u) + 1u)
static uint32_t gpio_stub_mem[STUB_WORDS];

static void reset_stub(void)
{
    memset((void *)gpio_stub_mem, 0, sizeof(gpio_stub_mem));
    /* Reset: DIR all 1 (inputs), OUT 0 */
    gpio_stub_mem[GPIO_DIR01_OFFSET / 4u] = 0xFFFFFFFFu;
    gpio_stub_mem[GPIO_OUT_DATA01_OFFSET / 4u] = 0x00000000u;
}

/* MMIO redirect by pointing base to the array address */
int main(void)
{
    bool v;

    reset_stub();
    gpio_data_init((uintptr_t)gpio_stub_mem);

    /* Emulate: make pins 0..3 outputs by clearing DIR bits */
    gpio_stub_mem[GPIO_DIR01_OFFSET / 4u] &= ~(0xFu);

    /* set/clear/write operations update OUT latch and should reflect in IN (since outputs) */
    if (gpio_set(1u) != GPIO_DATA_OK) { printf("ERR set(1)\n"); return 1; }
    if (gpio_read_out(1u, &v) != GPIO_DATA_OK || !v) { printf("ERR out(1)!=1\n"); return 1; }
    /* simulate IN mirrors OUT for outputs */
    gpio_stub_mem[GPIO_IN_DATA01_OFFSET / 4u] = gpio_stub_mem[GPIO_OUT_DATA01_OFFSET / 4u];
    if (gpio_read_in(1u, &v) != GPIO_DATA_OK || !v) { printf("ERR in(1)!=1\n"); return 1; }

    if (gpio_clear(1u) != GPIO_DATA_OK) { printf("ERR clear(1)\n"); return 1; }
    gpio_stub_mem[GPIO_IN_DATA01_OFFSET / 4u] = gpio_stub_mem[GPIO_OUT_DATA01_OFFSET / 4u];
    if (gpio_read_out(1u, &v) != GPIO_DATA_OK || v) { printf("ERR out(1)!=0\n"); return 1; }

    if (gpio_write(2u, true) != GPIO_DATA_OK) { printf("ERR write(2,1)\n"); return 1; }
    gpio_stub_mem[GPIO_IN_DATA01_OFFSET / 4u] = gpio_stub_mem[GPIO_OUT_DATA01_OFFSET / 4u];
    if (gpio_read_in(2u, &v) != GPIO_DATA_OK || !v) { printf("ERR in(2)!=1\n"); return 1; }

    if (gpio_toggle(2u) != GPIO_DATA_OK) { printf("ERR toggle(2)\n"); return 1; }
    gpio_stub_mem[GPIO_IN_DATA01_OFFSET / 4u] = gpio_stub_mem[GPIO_OUT_DATA01_OFFSET / 4u];
    if (gpio_read_in(2u, &v) != GPIO_DATA_OK || v) { printf("ERR in(2)!=0 after toggle\n"); return 1; }

    /* Input pin read: pin 10 remains input, drive it via IN register to emulate external level */
    gpio_stub_mem[GPIO_DIR01_OFFSET / 4u] |= (1u << 10u);
    gpio_stub_mem[GPIO_IN_DATA01_OFFSET / 4u] |= (1u << 10u);
    if (gpio_read_in(10u, &v) != GPIO_DATA_OK || !v) { printf("ERR in(10)!=1 (input read)\n"); return 1; }

    /* Bounds and null checks */
    if (gpio_set(32u) != GPIO_DATA_ERR_PARAM) { printf("ERR expected param err for pin32 set\n"); return 1; }
    if (gpio_read_in(1u, (bool *)0) != GPIO_DATA_ERR_PARAM) { printf("ERR expected param err for NULL read_in\n"); return 1; }

    printf("GPIO_DATA_IO tests PASS\n");
    return 0;
}
