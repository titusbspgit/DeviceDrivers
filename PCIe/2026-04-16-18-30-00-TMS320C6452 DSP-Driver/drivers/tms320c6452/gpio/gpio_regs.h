/* SPDX-License-Identifier: MIT */
/* File: drivers/tms320c6452/gpio/gpio_regs.h */
/* TMS320C6452 DSP GPIO Register Offsets and Bitfields */
#ifndef TMS320C6452_GPIO_REGS_H
#define TMS320C6452_GPIO_REGS_H

#include <stdint.h>

#define TMS320C6452_GPIO_BASE_DEFAULT   (0xA1008000u)

/* Register Offsets (from GPIO base) */
#define GPIO_PID_OFFSET                 (0x00u)
#define GPIO_BINTEN_OFFSET              (0x08u)
/* 0x0C reserved */
#define GPIO_DIR01_OFFSET               (0x10u)
#define GPIO_OUT_DATA01_OFFSET          (0x14u)
#define GPIO_SET_DATA01_OFFSET          (0x18u)
#define GPIO_CLR_DATA01_OFFSET          (0x1Cu)
#define GPIO_IN_DATA01_OFFSET           (0x20u)
#define GPIO_SET_RIS_TRIG01_OFFSET      (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET      (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET      (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET      (0x30u)
#define GPIO_INTSTAT01_OFFSET           (0x34u)

/* BINTEN bitfields */
#define GPIO_BINTEN_EN0_MASK            (0x00000001u)
#define GPIO_BINTEN_EN1_MASK            (0x00000002u)

/* Helpers */
#define GPIO_PIN_BIT(pin)               ((uint32_t)1u << (uint32_t)((pin) & 31u))

#endif /* TMS320C6452_GPIO_REGS_H */
