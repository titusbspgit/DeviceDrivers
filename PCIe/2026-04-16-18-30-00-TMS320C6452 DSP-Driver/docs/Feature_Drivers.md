# Feature Drivers Summary (TMS320C6452 DSP GPIO)

Base address: 0xA1008000

Implemented features (from Featurelist.docx) and API mapping:
- GPIO banked architecture: transparent in all APIs (32 pins total)
- Pin multiplexing control: hooks tmsgpio_platform_configure_pinmux()
- Endianness independence: handled by HAL reg_access
- Direction control (DIR01): tmsgpio_config_pin(), tmsgpio_config_mask()
- Output drive control (OUT/SET/CLR): tmsgpio_write_pin(), tmsgpio_set_mask(), tmsgpio_clear_mask()
- Input sampling (IN_DATA01): tmsgpio_read_pin(), tmsgpio_read_port()
- Per-pin edge-triggered interrupts: tmsgpio_irq_config_edge()
- Per-bank interrupt enable (BINTEN): tmsgpio_irq_bank_enable()
- Interrupt status and W1C (INTSTAT01): tmsgpio_irq_status(), tmsgpio_irq_clear()
- EDMA event support from GPIO: gated via BINTEN (routing is platform-specific)
- Clock gating and frequency (PSC/LPSC): hooks tmsgpio_platform_enable_psc()/disable()
- Low-power behavior via PSC: tmsgpio_enter_low_power(), tmsgpio_exit_low_power()
- Reset behavior: cleared INTs during init, comments for PSC/hardware resets
- Initialization sequence: tmsgpio_init()
- Emulation suspend behavior: documented in README
- Peripheral Identification (PID): tmsgpio_get_pid()
- Concurrency-safe set/clear operations: provided via SET/CLR
- Edge detection synchronized to GPIO clock: noted in comments
- Open-drain interaction: documented behavior via IN_DATA01
- No interrupt multiplexing with other functions: integration note
- Register map and offsets: drivers/tms320c6452/gpio/gpio_regs.h
- Controller base address: configurable via tmsgpio_init()
- Error handling features: API returns error codes
- Security features: documented at platform layer

## Build and Usage
- Include headers from hal/ and gpio/.
- Initialize with tmsgpio_init().
- Configure directions and edges before enabling bank interrupts.
- Clear INTs using W1C before exit from ISR.
