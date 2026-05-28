#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "core_driver.h"
#include "keystone_pcie_module_initialization_sequence_rc_mode.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();

    struct kpm_init_rc_params p = { .request_gen2 = true, .common_clock = true };
    int rc = kpm_init_rc_sequence(api, &p);
    (void)rc;
    return 0;
}
