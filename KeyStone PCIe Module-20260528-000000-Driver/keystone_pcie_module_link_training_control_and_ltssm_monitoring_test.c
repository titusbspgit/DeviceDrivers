#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "core_driver.h"
#include "keystone_pcie_module_link_training_control_and_ltssm_monitoring.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();

    (void)kpm_ltssm_enable(api, true);
    int rc = kpm_wait_link_up(api, 1000000);
    (void)rc;
    return 0;
}
