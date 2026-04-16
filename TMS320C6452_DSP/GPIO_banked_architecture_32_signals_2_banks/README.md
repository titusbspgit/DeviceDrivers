# Feature: GPIO banked architecture (32 signals, 2 banks)

This sample validates the TMS320C6452 GPIO banked architecture using the common GPIO driver.

- Two banks of 16 pins each (GP[15:0], GP[31:16]).
- Tests drive pin 0 (bank 0) high and pin 16 (bank 1) low and confirm readback.

Build:

make

Run:

Execute `./test.elf` in your bare-metal harness or simulator environment. The test prints PASS/FAIL.

Dependencies:
- Common driver under `../../common`.
- PSC clock and pinmux assumed enabled via weak HAL defaults.
