/* RCI-ag-DrvGen Agent: Unit test for GPIO Direction driver */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452 DSP_GPIO_DIRECTION.h"

/* Simple stub memory to emulate GPIO register space (word addressed) */
#define STUB_WORDS  ((GPIO_IN_DATA01_OFFSET / 4u) + 1u)
static uint32_t gpio_stub_mem[STUB_WORDS];

static void reset_stub(void)
{
    memset((void *)gpio_stub_mem, 0, sizeof(gpio_stub_mem));
    /* Reset state per spec: DIR01 all ones = inputs */
    gpio_stub_mem[GPIO_DIR01_OFFSET / 4u] = 0xFFFFFFFFu;
}

static uint32_t rd_stub(uint32_t off)
{
    return gpio_stub_mem[off / 4u];
}

static void wr_stub(uint32_t off, uint32_t val)
{
    gpio_stub_mem[off / 4u] = val;
}

int main(void)
{
    bool is_out;

    reset_stub();
    gpio_dir_init((uintptr_t)gpio_stub_mem);

    /* Sanity: after reset, pin 5 is input */
    if (gpio_get_dir(5u, &is_out) != GPIO_DIR_OK) { printf("ERR: get_dir param fail\n"); return 1; }
    if (is_out) { printf("ERR: pin5 should be input after reset\n"); return 1; }

    /* Configure pin 5 to output */
    if (gpio_set_dir(5u, true) != GPIO_DIR_OK) { printf("ERR: set_dir->output failed\n"); return 1; }
    if (gpio_get_dir(5u, &is_out) != GPIO_DIR_OK) { printf("ERR: get_dir after output failed\n"); return 1; }
    if (!is_out) { printf("ERR: pin5 expected output\n"); return 1; }

    /* Revert pin 5 to input */
    if (gpio_set_dir(5u, false) != GPIO_DIR_OK) { printf("ERR: set_dir->input failed\n"); return 1; }
    if (gpio_get_dir(5u, &is_out) != GPIO_DIR_OK) { printf("ERR: get_dir after input failed\n"); return 1; }
    if (is_out) { printf("ERR: pin5 expected input\n"); return 1; }

    /* Edge pins */
    if (gpio_set_dir(0u, true) != GPIO_DIR_OK) { printf("ERR: pin0 output\n"); return 1; }
    if (gpio_get_dir(0u, &is_out) != GPIO_DIR_OK || !is_out) { printf("ERR: pin0 not output\n"); return 1; }
    if (gpio_set_dir(31u, false) != GPIO_DIR_OK) { printf("ERR: pin31 input\n"); return 1; }
    if (gpio_get_dir(31u, &is_out) != GPIO_DIR_OK || is_out) { printf("ERR: pin31 not input\n"); return 1; }

    /* RMW preservation test: only targeted bit changes */
    wr_stub(GPIO_DIR01_OFFSET, 0xAAAA5555u); /* arbitrary pattern */
    if (gpio_set_dir(7u, true) != GPIO_DIR_OK) { printf("ERR: set_dir(7, out)\n"); return 1; }
    if (rd_stub(GPIO_DIR01_OFFSET) != (0xAAAA5555u & ~(1u << 7u))) { printf("ERR: RMW clear mismatch\n"); return 1; }
    if (gpio_set_dir(7u, false) != GPIO_DIR_OK) { printf("ERR: set_dir(7, in)\n"); return 1; }
    if (rd_stub(GPIO_DIR01_OFFSET) != (0xAAAA5555u | (1u << 7u))) { printf("ERR: RMW set mismatch\n"); return 1; }

    /* Error handling */
    if (gpio_set_dir(32u, true) != GPIO_DIR_ERR_PARAM) { printf("ERR: expected param error for pin 32\n"); return 1; }
    if (gpio_get_dir(1u, (bool *)0) != GPIO_DIR_ERR_PARAM) { printf("ERR: expected param error for NULL ptr\n"); return 1; }

    printf("GPIO_DIRECTION tests PASS\n");
    return 0;
}
