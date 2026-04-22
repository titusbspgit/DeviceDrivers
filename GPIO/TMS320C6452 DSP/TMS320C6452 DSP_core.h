/*
 * TMS320C6452 DSP Core Driver Header
 * arch=arm, env=uboot, compiler=gcc, peripheral endianness=bigendian
 * No dynamic memory; C89/C99 portable; MISRA-style discipline.
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address for MMIO region */
#define TMS320C6452_DSP_BASE_ADDR (0xA1008000u)

/* Optional direct register accessor (raw, no endianness conversion) */
#define TMS320C6452_DSP_REG32(off) (*(volatile uint32_t *)((uintptr_t)TMS320C6452_DSP_BASE_ADDR + (uint32_t)(off)))

/* Compiler barrier */
#if defined(__GNUC__)
#define RCI_MEM_BARRIER() __asm__ __volatile__("" ::: "memory")
#else
#define RCI_MEM_BARRIER() do { } while (0)
#endif

/* Error codes */
typedef enum
{
    RCI_DRV_OK     = 0,
    RCI_DRV_EINVAL = 22,
    RCI_DRV_EIO    = 5
} rci_drv_status_t;

/* Core ops structure to be used by feature drivers */
typedef struct
{
    int32_t (*init)(uint32_t base);
    void    (*deinit)(void);
    int32_t (*reg_read)(uint32_t offset, uint32_t *val);
    int32_t (*reg_write)(uint32_t offset, uint32_t val);
    int32_t (*set_bits)(uint32_t offset, uint32_t mask);
    int32_t (*clear_bits)(uint32_t offset, uint32_t mask);
    int32_t (*update_bits)(uint32_t offset, uint32_t mask, uint32_t value);
    void    (*delay_us)(uint32_t usec);
} tms320c6452_dsp_core_ops_t;

/* Core API (implemented in ..._core_drv.c) */
int32_t tms320c6452_dsp_core_init(uint32_t base);
void    tms320c6452_dsp_core_deinit(void);
int32_t tms320c6452_dsp_core_reg_read(uint32_t offset, uint32_t *val);
int32_t tms320c6452_dsp_core_reg_write(uint32_t offset, uint32_t val);
int32_t tms320c6452_dsp_core_set_bits(uint32_t offset, uint32_t mask);
int32_t tms320c6452_dsp_core_clear_bits(uint32_t offset, uint32_t mask);
int32_t tms320c6452_dsp_core_update_bits(uint32_t offset, uint32_t mask, uint32_t value);
void    tms320c6452_dsp_core_delay_us(uint32_t usec);

extern const tms320c6452_dsp_core_ops_t tms320c6452_dsp_core_ops;

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
