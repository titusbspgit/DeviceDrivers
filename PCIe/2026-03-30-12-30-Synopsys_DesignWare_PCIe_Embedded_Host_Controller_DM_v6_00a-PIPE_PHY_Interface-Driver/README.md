Title: Synopsys DWC PCIe DM v6.00a – External PIPE PHY Integration

Contents
- Linux kernel platform driver (dw_pcie_pipe_phy.c)
- Public headers (include/)
- Test app (N/A: kernel-integrated; use sysfs/debugfs)
- DT binding YAML + example

Build (Out-of-tree)
- Ensure kernel headers are available and KDIR set
- Create a simple Kbuild:

obj-m += dw_pcie_pipe_phy.o
ccflags-y += -Werror -Wall -Wextra

dw_pcie_pipe_phy-y := src/dw_pcie_pipe_phy.o

make -C $(KDIR) M=$(PWD) modules

Usage
- Insert: insmod dw_pcie_pipe_phy.ko allow_writes=1
- DT must provide: reg, clocks (refclk), resets (pma, pipe), phys (pcie-phy), and properties: refclk-rate, ssc, pipe-version.
- Inspect caps: cat /sys/kernel/debug/dw_pcie_pipe/caps
- Adjust policy: echo 1 > /sys/devices/platform/<node>/ssc_enable (requires allow_writes=1)

DT Binding (YAML)
See: Documentation/devicetree/bindings/pci/synopsys,dw-pcie-dm-pipe-phy.yaml

Example Node
synopsys_pcie_pipe: pcie-pipe@f9000000 {
    compatible = "synopsys,dw-pcie-dm-pipe-phy";
    reg = <0x0 0xf9000000 0x0 0x2000>;
    clocks = <&clk_ref>;
    clock-names = "refclk";
    resets = <&rstc 12>, <&rstc 13>;
    reset-names = "pma", "pipe";
    phys = <&pcie_serdes>;
    phy-names = "pcie-phy";
    refclk-rate = <100000000>;
    ssc;
    pipe-version = <5>;
};
