/* Test application for single-feature driver: Direction control (DIR01) */
#include <stdio.h>
#include <stdint.h>
#include "tms320c6452_dsp_direction_control_dir01_drv.h"

static void print_result(const char *name, int rc)
{
    printf("%s: rc=%d\n", name, rc);
}

int main(void)
{
    tms320c6452_dsp_dir_ctx_t ctx;
    uint32_t dir;
    uint32_t is_in;
    int rc;
    const uint32_t test_pin = 3u; /* example */

    rc = tms320c6452_dsp_direction_control_dir01_init(&ctx, TMS320C6452_DSP_GPIO_BASE_DEFAULT);
    print_result("init", rc);
    if (rc != 0) { return rc; }

    /* Set entire mask to inputs, then drive selected pin to output and back */
    rc = tms320c6452_dsp_direction_control_dir01_write_mask(&ctx, 0xFFFFFFFFu);
    print_result("write_mask_all_inputs", rc);

    rc = tms320c6452_dsp_direction_control_dir01_get(&ctx, test_pin, &is_in);
    print_result("get_after_all_inputs", rc);
    printf("pin %u is_input=%u\n", test_pin, is_in);

    rc = tms320c6452_dsp_direction_control_dir01_set_output(&ctx, test_pin);
    print_result("set_output", rc);

    rc = tms320c6452_dsp_direction_control_dir01_get(&ctx, test_pin, &is_in);
    print_result("get_after_output", rc);
    printf("pin %u is_input=%u (expect 0)\n", test_pin, is_in);

    rc = tms320c6452_dsp_direction_control_dir01_set_input(&ctx, test_pin);
    print_result("set_input", rc);

    rc = tms320c6452_dsp_direction_control_dir01_read_mask(&ctx, &dir);
    print_result("read_mask", rc);
    printf("DIR01=0x%08X\n", dir);

    /* Edge case: invalid pin */
    rc = tms320c6452_dsp_direction_control_dir01_set_input(&ctx, 32u);
    print_result("set_input_invalid_pin", rc);

    rc = tms320c6452_dsp_direction_control_dir01_deinit(&ctx);
    print_result("deinit", rc);

    return 0;
}
