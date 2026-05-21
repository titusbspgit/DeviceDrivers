#include <stdint.h>
#include <stdio.h>
#include "tms320c6452_pin_multiplexing_control.h"

int main(void)
{
    int ret;
    ret = tms320c6452_pinmux_validate();
    if (ret != 0) {
        printf("pinmux_validate unsupported as expected (ret=%d)\n", ret);
    }

    ret = tms320c6452_pinmux_configure_default();
    if (ret != 0) {
        printf("pinmux_configure_default unsupported as expected (ret=%d)\n", ret);
    }

    uint32_t mux;
    ret = tms320c6452_pinmux_get(0u, &mux);
    if (ret != 0) {
        printf("pinmux_get unsupported as expected (ret=%d)\n", ret);
    }

    return 0;
}
