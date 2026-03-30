# Synopsys DWC PCIe DM v6.00a — Link Training and Equalization Debug

This feature provides a Linux kernel out-of-tree debug driver and a portable HAL to access DesignWare PCIe DM v6.00a link training and equalization controls.

Contents
- include/dw_pcie_eq_dbg.h: Public HAL API and register definitions
- src/dw_pcie_eq_dbg_hal.c: Portable C HAL (no Linux deps)
- src/dw_pcie_eq_dbg_linux.c: Linux kernel wrapper exposing sysfs/debugfs
- tests/dw_pcie_eq_tool.c: User tool talking to sysfs/debugfs

Build (kernel module, OOT)
- Create a Makefile:
  obj-m += dw_pcie_eq_dbg_linux.o
  KDIR ?= /lib/modules/$(shell uname -r)/build
  all:; $(MAKE) -C $(KDIR) M=$(PWD) modules
  clean:; $(MAKE) -C $(KDIR) M=$(PWD) clean

Load
- insmod dw_pcie_eq_dbg_linux.ko bdf=0000:00:00.0 allow_writes=1 retrain_timeout_ms=100 eq_timeout_ms=24

Sysfs (under /sys/bus/pci/devices/<BDF>/eq_dbg)
- gen3_related (ro/rw): 32-bit register; writes gated by allow_writes=1 (protected via DBI_RO_WR_EN)
- local_fs_lf (ro/rw): prints FS/LF and raw; store takes "FS LF"
- coeff_legal_status (ro): legality/status dword
- preset_index (wo): select preset index 0..10
- preset_tuple (ro/rw): read or program PRE/CUR/POST for selected preset (writes gated; protected writes)
- preset_vector (wo): set Preset Request Vector mask (bits 0..10)
- force_recovery (wo): write "1" to request LTSSM Recovery then wait for L0 (bounded by retrain_timeout_ms)

Debugfs
- /sys/kernel/debug/dw_pcie_eq/<BDF>/summary: dumps core registers and PSET[0..10]

Safety Notes
- Default is read-only (allow_writes=0). Enabling writes can disrupt the link.
- Program presets and FS/LF only with traffic quiesced. Prefer triggering SW EQ instead of hard forcing LTSSM states.
- Vendor-specific DLLP and PORT_FORCE advanced controls are intentionally not exposed via sysfs by default due to SoC-specific encodings. Gate via code changes if required.

User Tool
- Build: gcc -O2 -Wall -Wextra -o dw_pcie_eq_tool tests/dw_pcie_eq_tool.c
- Example: ./dw_pcie_eq_tool 0000:00:00.0 --get --fs=40 --lf=20 --pset 7 12 32 8 --pvec 0x0810 --recovery
