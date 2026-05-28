#include <stdint.h>
#include <stdbool.h>
#include "KeyStone_PCIe_ModuleLink_width_and_speed_x1_x2_Gen1_Gen2.h"

static volatile int g_passed = 0;
static volatile int g_failed = 0;
static void expect_int_eq(const char *name, int exp, int got)
{
    (void)name;
    if (exp == got) g_passed++; else g_failed++;
}

int main(void)
{
    kpm_core_api_t api = kpm_core_get_api();

    expect_int_eq("init", KPM_OK, kpm_linkws_init(&api));

    /* With unknown registers, functions should report ENOTSUP */
    expect_int_eq("set_adv_enotsup", KPM_ENOTSUP, kpm_linkws_set_advertised(&api, KPM_LINK_WIDTH_X1, KPM_LINK_SPEED_GEN1));

    kpm_link_width_t w; kpm_link_speed_t s;
    expect_int_eq("get_neg_enotsup", KPM_ENOTSUP, kpm_linkws_get_negotiated(&api, &w, &s));

    return (g_failed == 0) ? 0 : 1;
}
