#include "../include/gpio_feat_13.h"
#include <stdio.h>
void gpio_feat13_reset_reinit(tms_gpio_ctx_t *ctx){ tms_gpio_feature_reset_reinit_sequence(ctx); printf("[FEAT13] Reinit after reset sequence executed.\n"); }
