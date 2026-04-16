#include "../include/EDMA_event_support_from_GPIO_api.h"

int EDMA_event_support_from_GPIO_feature_run(void)
{
    /* EDMA routing is platform-specific; here we only verify BINTEN can be enabled */
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    if (gpio_irq_bank_enable(0u, true) != GPIO_STATUS_OK) { return -2; }
    return 0;
}
