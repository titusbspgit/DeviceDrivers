#include <stdint.h>
#include "core_TMS320C6452 DSP.h"
#include "tms320c6452_dsp_gpio_direction_control_(dir01).h"

/* Minimal standalone test (no framework) */
int main(void){
    core_api_t api;
    core_get_api(&api);

    if(tms320c6452_dsp_gpio_direction_control_dir01_init(&api) != 0){
        return -1;
    }

    /* Read original configuration */
    uint32_t orig = tms320c6452_dsp_gpio_direction_control_dir01_read();

    /* Program a test pattern (semantics UNKNOWN: 1 may indicate input or output) */
    (void)tms320c6452_dsp_gpio_direction_control_dir01_write(0xAAAAAAAAu);

    /* Modify a subset via masked helpers */
    (void)tms320c6452_dsp_gpio_direction_control_dir01_set_bits(0x0000FFFFu);
    (void)tms320c6452_dsp_gpio_direction_control_dir01_clear_bits(0x000000FFu);

    /* Read back */
    uint32_t now = tms320c6452_dsp_gpio_direction_control_dir01_read();

    /* Restore original configuration */
    (void)tms320c6452_dsp_gpio_direction_control_dir01_write(orig);

    /* Prevent compiler from optimizing away reads */
    (void)now;

    return 0;
}
