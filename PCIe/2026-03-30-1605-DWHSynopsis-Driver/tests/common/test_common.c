#include "test_common.h"

/* Backing store for fake DBI in tests (4KB) */
volatile uint32_t g_fake_dbi[1024];
