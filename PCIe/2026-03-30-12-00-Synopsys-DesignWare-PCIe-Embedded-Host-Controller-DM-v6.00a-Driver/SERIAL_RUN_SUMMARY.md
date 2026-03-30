Run summary (RCI-ag-DrvGen Agent)

- Feature 1 (Flit Mode): driver, header, and test added. Assumptions noted for DEV3 and Link Control/Status3 register layout.
- Feature 2 (PIPE PHY): viewport accessors and simple bring-up helpers added; test simulates viewport and PHY-ready.
- Feature 3 (SPCIE): discovery and control/status accessors added; test covers EQ request and W1C lane error clearing.
- Feature 4 (EQ Controls): Gen3 FS/LF and preset/programming helpers; vendor DLLP trigger; retrain. Test covers basic R/W.
- Feature 5 (ASPM): standard Link Control programming and DWC tuning knobs. Test verifies bit toggling.

Notes:
- Tests are platform-agnostic and do not touch real hardware; they validate API semantics and register access patterns.
- Field positions for some vendor registers are assumptions; align with your configured IP manual for production use.
