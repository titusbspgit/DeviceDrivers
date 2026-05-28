#include <stdint.h>
#include <stdio.h>
#include "core_driver.h"
#include "keystone_pcie_module_msi_generation_ep_mode.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();

    /* Program dummy MSI target (host must assign real values) */
    (void)kpm_msi_program_target(api, 0x00000000u, 0x00000000u, 0x0000u);

    /* Trigger not implemented; expect -1 */
    (void)kpm_msi_trigger_vector(api, 0u);
    return 0;
}
