#include "../include/gpio_feat_19.h"
#include <stdio.h>
void gpio_feat19_log_readback(tms_gpio_ctx_t *ctx, uint32_t pin){ uint32_t v=tms_gpio_read_inputs(ctx); printf("[FEAT19] Open-drain note. Pin %lu readback=0x%u (wired-logic if OD).\n", (unsigned long)pin, (unsigned)((v>>pin)&1u)); }
