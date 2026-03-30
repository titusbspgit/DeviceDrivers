# MISRA C:2012 Deviations (documented)

- Use of __builtin_bswap16/32 and inline assembly memory barriers: controlled deviation for performance and determinism.
- Assumed bit positions for certain fields (LINK_CTRL3_FLIT_MODE_ENABLE, LINK_STATUS2_FLIT_MODE_ACTIVE, PORT_FORCE bits) are made configurable via macros. Default values align with common DWC DM v6.00a builds. Users must confirm with their RTL register maps.
- Polling loops with timeouts in PHY viewport and FLIT enable sequences: loops are bounded and return TIMEOUT on failure; compliant with single-threaded bare-metal constraints.
- Weak delay function for portability; platforms should override to ensure timing guarantees where needed.
