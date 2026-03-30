#ifndef DW_PCIE_CFG_H
#define DW_PCIE_CFG_H

/* Configuration for endianness and platform */
#define DW_PCIE_CPU_BIG_ENDIAN        1u     /* Target CPU endianness */
#define DW_PCIE_DEVICE_LITTLE_ENDIAN  1u     /* DWC DBI register endianness */

/* Optional: number of iATU regions supported (minimum assumed) */
#define DW_PCIE_IATU_MAX_OB_REGIONS   8u

/* Simple barrier macros (ARM) */
#if defined(__arm__) || defined(__aarch64__)
#define DW_DMB() __asm__ volatile("dmb sy" ::: "memory")
#define DW_DSB() __asm__ volatile("dsb sy" ::: "memory")
#define DW_ISB() __asm__ volatile("isb sy" ::: "memory")
#else
#define DW_DMB() do { } while (0)
#define DW_DSB() do { } while (0)
#define DW_ISB() do { } while (0)
#endif

#endif /* DW_PCIE_CFG_H */
