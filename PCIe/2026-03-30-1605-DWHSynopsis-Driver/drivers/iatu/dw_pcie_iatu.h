#ifndef DW_PCIE_IATU_H
#define DW_PCIE_IATU_H

#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_hal.h"
#include "dw_pcie_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DW_IATU_DIR_OUTBOUND = 0,
    DW_IATU_DIR_INBOUND  = 1
} dw_iatu_dir_t;

typedef enum {
    DW_IATU_TLP_MEM  = DW_IATU_TYPE_MEM,
    DW_IATU_TLP_IO   = DW_IATU_TYPE_IO,
    DW_IATU_TLP_CFG0 = DW_IATU_TYPE_CFG0,
    DW_IATU_TLP_CFG1 = DW_IATU_TYPE_CFG1
} dw_iatu_tlp_t;

/* Program an outbound iATU region using the viewport model */
int dw_pcie_iatu_program_ob(dw_pcie_dev_t *dev,
                            uint32_t region_index,
                            dw_iatu_tlp_t tlp_type,
                            uint64_t cpu_addr,
                            uint64_t pci_addr,
                            uint64_t size);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_IATU_H */
