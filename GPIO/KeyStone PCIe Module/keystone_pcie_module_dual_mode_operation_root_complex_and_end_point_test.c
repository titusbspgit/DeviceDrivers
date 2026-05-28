#include <stdint.h>
#include <stdbool.h>
#include "keystone_pcie_module_dual_mode_operation_root_complex_and_end_point.h"

/* Minimal test harness without I/O; use volatile flags for observation */
static volatile int g_tests_passed = 0;
static volatile int g_tests_failed = 0;

static void test_expect_eq_int(const char *name, int exp, int got)
{
    if (exp == got) {
        g_tests_passed++;
    } else {
        (void)name; /* suppress unused warning */
        g_tests_failed++;
    }
}

int main(void)
{
    int rc;

    rc = kpm_dual_mode_init();
    test_expect_eq_int("init", KPM_OK, rc);

    /* With UNKNOWN registers, configuration is expected to be not supported */
    rc = kpm_dual_mode_configure(KPM_PCIE_MODE_EP);
    test_expect_eq_int("cfg_ep_enotsup", KPM_ENOTSUP, rc);

    rc = kpm_dual_mode_configure(KPM_PCIE_MODE_RC);
    test_expect_eq_int("cfg_rc_enotsup", KPM_ENOTSUP, rc);

    rc = kpm_dual_mode_enable_ltssm();
    test_expect_eq_int("ltssm_en_enotsup", KPM_ENOTSUP, rc);

    rc = kpm_dual_mode_disable_ltssm();
    test_expect_eq_int("ltssm_dis_enotsup", KPM_ENOTSUP, rc);

    uint32_t state_raw = 0u; bool in_l0 = false;
    rc = kpm_dual_mode_get_link_state(&state_raw, &in_l0);
    test_expect_eq_int("get_lstate_enotsup", KPM_ENOTSUP, rc);

    /* Final status in volatile vars for external observation */
    return (g_tests_failed == 0) ? 0 : 1;
}
