#include "core_KeyStone_PCIe_Module.h"

/* This TU intentionally relies on header-only inline helpers to avoid
 * pulling architecture-specific libraries in baremetal builds. */

/* Provide a weak symbol to allow link-time verification that the core is present */
__attribute__((weak)) void kpm_core_present_marker(void) { (void)0; }
