# PIPE PHY Viewport

- Accesses external PIPE PHY registers through DWC viewport at 0xB70/0xB74.
- Provides parameterizable control field masks to adapt to synthesis differences.
- API: pcie_phy_read/write/basic_init with bounded timeouts.
