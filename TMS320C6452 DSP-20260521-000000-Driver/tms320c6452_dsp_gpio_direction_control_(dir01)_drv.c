#include "tms320c6452_dsp_gpio_direction_control_(dir01).h"

static core_api_t s_core;

int tms320c6452_dsp_gpio_direction_control_dir01_init(const core_api_t* api){
    if(!api) return -1;
    s_core = *api; /* struct copy */
    /* No hardware side effects required for DIR01 init */
    return 0;
}

uint32_t tms320c6452_dsp_gpio_direction_control_dir01_read(void){
    return s_core.read32(DIR01_ADDR);
}

int tms320c6452_dsp_gpio_direction_control_dir01_write(uint32_t value){
    s_core.write32(DIR01_ADDR, value);
    return 0;
}

int tms320c6452_dsp_gpio_direction_control_dir01_set_bits(uint32_t mask){
    s_core.rmw32(DIR01_ADDR, mask, 0u);
    return 0;
}

int tms320c6452_dsp_gpio_direction_control_dir01_clear_bits(uint32_t mask){
    s_core.rmw32(DIR01_ADDR, 0u, mask);
    return 0;
}
