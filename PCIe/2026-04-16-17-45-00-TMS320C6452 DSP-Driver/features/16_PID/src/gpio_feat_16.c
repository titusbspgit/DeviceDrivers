#include "../include/gpio_feat_16.h"
#include <stdio.h>
uint32_t gpio_feat16_read_pid(tms_gpio_ctx_t *ctx){ uint32_t pid=tms_gpio_get_pid(ctx); printf("[FEAT16] PID=0x%08lX\n", (unsigned long)pid); return pid; }
