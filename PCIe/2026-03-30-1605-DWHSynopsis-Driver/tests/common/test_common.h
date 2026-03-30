#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_hal.h"

/* Simple assert macro for bare-metal test logs */
#define TEST_ASSERT(cond) do { if (!(cond)) { printf("ASSERT FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); return -1; } } while (0)
#define TEST_INFO(fmt, ...) do { printf("[INFO] " fmt "\n", ##__VA_ARGS__); } while (0)

/* Fake DBI memory window for tests */
extern volatile uint32_t g_fake_dbi[1024];

static inline uintptr_t fake_dbi_base(void)
{
    return (uintptr_t)&g_fake_dbi[0];
}

#endif /* TEST_COMMON_H */
