# Feature: Pin multiplexing control

This feature demonstrates delegating GPIO pin configuration to a platform HAL via the weak hook `gpio_hal_configure_pinmux()`.

The sample test overrides the weak symbol to always return success. On real hardware, provide a platform-specific implementation that programs the SoC pinmux registers to select GPIO function for the given pin.
