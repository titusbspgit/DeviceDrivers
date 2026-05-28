#include <stdint.h>
#include <stdbool.h>
#include "KeyStone_PCIe_ModuleDual_mode_operation_RC_EP.h"

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

    /* Initialization */
    expect_int_eq("init", KPM_OK, kpm_dualmode_init(&api));

    /* With unknown registers, functions should report ENOTSUP */
    expect_int_eq("set_mode_enotsup", KPM_ENOTSUP, kpm_dualmode_set_mode(&api, KPM_PCIE_MODE_RC));

    kpm_pcie_mode_t mode;
    expect_int_eq("get_mode_enotsup", KPM_ENOTSUP, kpm_dualmode_get_mode(&api, &mode));

    expect_int_eq("ltssm_en_enotsup", KPM_ENOTSUP, kpm_dualmode_enable_ltssm(&api));
    expect_int_eq("ltssm_dis_enotsup", KPM_ENOTSUP, kpm_dualmode_disable_ltssm(&api));

    uint32_t state_raw = 0; bool in_l0 = false;
    expect_int_eq("get_state_enotsup", KPM_ENOTSUP, kpm_dualmode_get_link_state(&api, &state_raw, &in_l0));

    expect_int_eq("wait_l0_enotsup", KPM_ENOTSUP, kpm_dualmode_wait_l0(&api, 1000u));

    return (g_failed == 0) ? 0 : 1;
}
