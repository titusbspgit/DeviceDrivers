/* RCI-ag-DrvGen Agent: Unit test for GPIO Direction driver */
#include <stdio.h>
#include <string.h>
#include "TMS320C6452 DSP_GPIO_DIRECTION.h"

/* Simple stub memory to emulate GPIO registers */
static struct {
    uint32_t pad0[4];
    uint32_t DIR01;      /* 0x10 */
    uint32_t OUT_DATA01; /* 0x14 */
    uint32_t SET_DATA01; /* 0x18 */
    uint32_t CLR_DATA01; /* 0x1C */
    uint32_t IN_DATA01;  /* 0x20 */
} gpio_stub_mem;

static void reset_stub(void)
{
    memset(&gpio_stub_mem, 0, sizeof(gpio_stub_mem));
    gpio_stub_mem.DIR01 = 0xFFFFFFFFu; /* reset: inputs */
}

int main(void)
{
    bool is_out;

    reset_stub();
    gpio_dir_init((uintptr_t)&gpio_stub_mem);

    /* Check initial state: pin 5 should be input */
    if (gpio_get_dir(5u, &is_out) != GPIO_DIR_OK) { printf("ERR: get_dir invalid param\n"); return 1; }
    if (is_out) { printf("ERR: pin5 expected input after reset\n"); return 1; }

    /* Set pin 5 to output */
    if (gpio_set_dir(5u, true) != GPIO_DIR_OK) { printf("ERR: set_dir failed\n"); return 1; }
    if (gpio_get_dir(5u, &is_out) != GPIO_DIR_OK) { printf("ERR: get_dir failed\n"); return 1; }
    if (!is_out) { printf("ERR: pin5 expected output after set\n"); return 1; }

    /* Revert pin 5 to input */
    if (gpio_set_dir(5u, false) != GPIO_DIR_OK) { printf("ERR: set_dir failed (to input)\n"); return 1; }
    if (gpio_get_dir(5u, &is_out) != GPIO_DIR_OK) { printf("ERR: get_dir failed (after input)\n"); return 1; }
    if (is_out) { printf("ERR: pin5 expected input after clear\n"); return 1; }

    /* Edge cases */
    if (gpio_set_dir(32u, true) != GPIO_DIR_ERR_PARAM) { printf("ERR: expected param error for pin 32\n"); return 1; }
    if (gpio_get_dir(0u, NULL) != GPIO_DIR_ERR_PARAM) { printf("ERR: expected param error for NULL ptr\n"); return 1; }

    printf("GPIO_DIRECTION tests PASS\n");
    return 0;
}
