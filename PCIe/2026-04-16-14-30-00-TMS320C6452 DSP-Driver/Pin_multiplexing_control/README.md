Validates pin multiplexing integration by relying on the HAL pinmux hook inside gpio_set_direction(). The test overrides the hook to always succeed and verifies direction change occurs.
