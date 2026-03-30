/*
 * MIT License
 */
#include "drivers/pcie/features/ltssm_eq/pcie_ltssm_eq.h"

pcie_status_t pcie_eq_set_local_fs_lf(pcie_dbi_t* ctx, uint8_t fs, uint8_t lf)
{
    if ((ctx == NULL) || (fs > 63u) || (lf > 63u)) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t v = ((uint32_t)fs << GEN3_EQ_FS_POS) & GEN3_EQ_FS_MASK;
    v |= ((uint32_t)lf << GEN3_EQ_LF_POS) & GEN3_EQ_LF_MASK;
    pcie_write32(ctx, DWC_GEN3_EQ_LOCAL_FS_LF_OFF, v);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_eq_program_preset_map(pcie_dbi_t* ctx, uint32_t index, uint32_t coef_dw)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    pcie_write32(ctx, DWC_GEN3_EQ_PRESET_INDEX_OFF, index);
    pcie_write32(ctx, DWC_GEN3_EQ_PSET_COEF_MAP_OFF, coef_dw);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_eq_get_status(const pcie_dbi_t* ctx, uint32_t* out_status)
{
    if ((ctx == NULL) || (out_status == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    *out_status = pcie_read32(ctx, DWC_GEN3_EQ_STATUS_OFF);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_port_send_vendor_dllp(pcie_dbi_t* ctx, uint32_t payload)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    pcie_write32(ctx, DWC_PORT_VSDLLP_OFF, payload);
    pcie_rmw32(ctx, DWC_PORT_FORCE_OFF, 0u, PORT_FORCE_SEND_VS_DLLP);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_port_force_ltssm_state(pcie_dbi_t* ctx, uint8_t state_code, bool enable_force)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t set = 0u;
    uint32_t clr = PORT_FORCE_LTSSM_FORCE_EN | PORT_FORCE_LTSSM_STATE_MASK;
    if (enable_force) {
        set |= (((uint32_t)state_code << PORT_FORCE_LTSSM_STATE_POS) & PORT_FORCE_LTSSM_STATE_MASK);
        set |= PORT_FORCE_LTSSM_FORCE_EN;
    }
    pcie_rmw32(ctx, DWC_PORT_FORCE_OFF, clr, set);
    return PCIE_STATUS_OK;
}
