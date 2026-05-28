#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "core_driver.h"
#include "keystone_pcie_module_msi_reception_rc_mode.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();
    bool act = false;

    (void)kpm_msi_enable_vector(api, 0);

    /* Poll once and clear if set */
    (void)kpm_msi_poll_and_clear(api, 0, &act);

    /* For test injection (if supported), doorbell vector 0 */
    api->write32(REG_MSI_DOORBELL_OFF, 0u);
    (void)kpm_msi_poll_and_clear(api, 0, &act);

    (void)kpm_msi_disable_vector(api, 0);
    return 0;
}
