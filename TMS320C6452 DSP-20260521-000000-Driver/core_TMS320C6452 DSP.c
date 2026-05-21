#include "core_TMS320C6452 DSP.h"

static uint32_t core_read32_impl(uintptr_t addr){
    core_mb();
    volatile uint32_t* p = (volatile uint32_t*)addr;
    uint32_t v = *p;
    return core_be32_to_cpu(v);
}

static void core_write32_impl(uintptr_t addr, uint32_t value){
    core_mb();
    volatile uint32_t* p = (volatile uint32_t*)addr;
    *p = core_cpu_to_be32(value);
    core_mb();
}

static void core_rmw32_impl(uintptr_t addr, uint32_t set_mask, uint32_t clear_mask){
    uint32_t v = core_read32_impl(addr);
    v &= ~clear_mask;
    v |= set_mask;
    core_write32_impl(addr, v);
}

static void core_delay_cycles_impl(uint32_t cycles){
    volatile uint32_t i = cycles;
    while(i--){ __asm__ __volatile__("" ::: "memory"); }
}

void core_get_api(core_api_t* api){
    if(!api) return;
    api->read32 = core_read32_impl;
    api->write32 = core_write32_impl;
    api->rmw32 = core_rmw32_impl;
    api->delay_cycles = core_delay_cycles_impl;
}
