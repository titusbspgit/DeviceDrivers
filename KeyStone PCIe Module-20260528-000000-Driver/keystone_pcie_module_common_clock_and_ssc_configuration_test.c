#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "core_driver.h"
#include "keystone_pcie_module_common_clock_and_ssc_configuration.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();

    (void)kpm_set_common_clock(api, true);
    bool cc = kpm_get_common_clock(api);
    (void)cc;
    return 0;
}
