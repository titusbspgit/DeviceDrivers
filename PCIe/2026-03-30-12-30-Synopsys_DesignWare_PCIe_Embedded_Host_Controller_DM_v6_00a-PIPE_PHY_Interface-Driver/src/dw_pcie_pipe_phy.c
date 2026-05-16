// SPDX-License-Identifier: BSD-3-Clause
/*
 * Synopsys DWC PCIe DM v6.00a - External PIPE PHY Linux shim
 * - Platform driver using DT
 * - Integrates Linux PHY framework (optional)
 * - Exposes sysfs attributes for refclk_hz, ssc_enable, pipe_gen, caps
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

#include "../include/dw_pcie_pipe_phy.h"
#include "../include/dw_pcie_pipe_prb.h"

static bool allow_writes;
module_param(allow_writes, bool, 0644);
MODULE_PARM_DESC(allow_writes, "Enable write access to PHY via PRB and policy knobs");

static struct dentry *dbg_dir;

static inline u32 rd(struct dw_pcie_pipe_phy *ctx, u32 off)
{
    return readl(ctx->base + off);
}

static inline void wr(struct dw_pcie_pipe_phy *ctx, u32 off, u32 val)
{
    writel(val, ctx->base + off);
}

static int prb_wait_not_busy(struct dw_pcie_pipe_phy *ctx, u32 timeout_us)
{
    u32 remaining = timeout_us ? timeout_us : DW_PCIE_PRB_BUSY_TIMEOUT_US_DEFAULT;
    while (remaining) {
        u32 ctl = rd(ctx, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);
        if (!(ctl & DW_PCIE_PRB_CTLSTS_GO_BUSY))
            return 0;
        udelay(DW_PCIE_PRB_BUSY_POLL_STEP_US);
        remaining = (remaining > DW_PCIE_PRB_BUSY_POLL_STEP_US) ?
            remaining - DW_PCIE_PRB_BUSY_POLL_STEP_US : 0u;
    }
    return -ETIMEDOUT;
}

static void prb_clear_errors(struct dw_pcie_pipe_phy *ctx)
{
    u32 ctl = rd(ctx, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);
    u32 clr = ctl & (DW_PCIE_PRB_CTLSTS_TIMEOUT | DW_PCIE_PRB_CTLSTS_ERROR);
    if (clr)
        wr(ctx, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF, clr);
}

int dw_pcie_prb_read(struct dw_pcie_pipe_phy *ctx, u16 prb_addr, u32 *data_out)
{
    int ret;
    if (!ctx || !data_out)
        return -EINVAL;

    ret = prb_wait_not_busy(ctx, ctx->policy.prb_timeout_us);
    if (ret)
        return ret;
    prb_clear_errors(ctx);

    writel(dw_pcie_prb_ctl_build(prb_addr, false) | DW_PCIE_PRB_CTLSTS_GO_BUSY,
           ctx->base + DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);

    ret = prb_wait_not_busy(ctx, ctx->policy.prb_timeout_us);
    if (ret)
        return ret;

    {
        u32 ctl = rd(ctx, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);
        if (ctl & (DW_PCIE_PRB_CTLSTS_TIMEOUT | DW_PCIE_PRB_CTLSTS_ERROR)) {
            prb_clear_errors(ctx);
            return -EIO;
        }
    }

    *data_out = rd(ctx, DW_PCIE_PHY_VIEWPORT_DATA_OFF);
    return 0;
}
EXPORT_SYMBOL_GPL(dw_pcie_prb_read);

int dw_pcie_prb_write(struct dw_pcie_pipe_phy *ctx, u16 prb_addr, u32 data_in)
{
    int ret;
    if (!ctx)
        return -EINVAL;
    if (!allow_writes)
        return -EPERM;

    ret = prb_wait_not_busy(ctx, ctx->policy.prb_timeout_us);
    if (ret)
        return ret;
    prb_clear_errors(ctx);

    wr(ctx, DW_PCIE_PHY_VIEWPORT_DATA_OFF, data_in);
    writel(dw_pcie_prb_ctl_build(prb_addr, true) | DW_PCIE_PRB_CTLSTS_GO_BUSY,
           ctx->base + DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);

    ret = prb_wait_not_busy(ctx, ctx->policy.prb_timeout_us);
    if (ret)
        return ret;

    {
        u32 ctl = rd(ctx, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);
        if (ctl & (DW_PCIE_PRB_CTLSTS_TIMEOUT | DW_PCIE_PRB_CTLSTS_ERROR)) {
            prb_clear_errors(ctx);
            return -EIO;
        }
    }
    return 0;
}
EXPORT_SYMBOL_GPL(dw_pcie_prb_write);

int dw_pcie_pipe_dump_caps(struct dw_pcie_pipe_phy *ctx,
                           u8 *tx_min_wr_buf_depth,
                           u8 *rx_wr_buf_depth,
                           bool *garbage_data_mode)
{
    u32 v;
    if (!ctx || !tx_min_wr_buf_depth || !rx_wr_buf_depth || !garbage_data_mode)
        return -EINVAL;
    v = rd(ctx, DW_PCIE_PIPE_RELATED_OFF);
    *tx_min_wr_buf_depth = (u8)((v & DW_PCIE_PIPE_RELATED_TX_MSG_MIN_WR_BUF_DEPTH_MASK) >>
                                DW_PCIE_PIPE_RELATED_TX_MSG_MIN_WR_BUF_DEPTH_SHIFT);
    *rx_wr_buf_depth = (u8)((v & DW_PCIE_PIPE_RELATED_RX_MSG_WR_BUF_DEPTH_MASK) >>
                            DW_PCIE_PIPE_RELATED_RX_MSG_WR_BUF_DEPTH_SHIFT);
    *garbage_data_mode = !!(v & DW_PCIE_PIPE_RELATED_GARBAGE_DATA_MODE);
    return 0;
}
EXPORT_SYMBOL_GPL(dw_pcie_pipe_dump_caps);

/* sysfs attributes */
static ssize_t refclk_hz_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct dw_pcie_pipe_phy *ctx = dev_get_drvdata(dev);
    return sysfs_emit(buf, "%u\n", ctx->policy.refclk_hz);
}
static ssize_t refclk_hz_store(struct device *dev, struct device_attribute *attr,
                               const char *buf, size_t count)
{
    struct dw_pcie_pipe_phy *ctx = dev_get_drvdata(dev);
    unsigned long v;
    if (!allow_writes)
        return -EPERM;
    if (kstrtoul(buf, 0, &v))
        return -EINVAL;
    ctx->policy.refclk_hz = (u32)v;
    return count;
}
static DEVICE_ATTR_RW(refclk_hz);

static ssize_t ssc_enable_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct dw_pcie_pipe_phy *ctx = dev_get_drvdata(dev);
    return sysfs_emit(buf, "%u\n", ctx->policy.ssc_enable ? 1 : 0);
}
static ssize_t ssc_enable_store(struct device *dev, struct device_attribute *attr,
                                const char *buf, size_t count)
{
    struct dw_pcie_pipe_phy *ctx = dev_get_drvdata(dev);
    unsigned long v;
    if (!allow_writes)
        return -EPERM;
    if (kstrtoul(buf, 0, &v))
        return -EINVAL;
    ctx->policy.ssc_enable = (v != 0);
    return count;
}
static DEVICE_ATTR_RW(ssc_enable);

static ssize_t pipe_gen_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct dw_pcie_pipe_phy *ctx = dev_get_drvdata(dev);
    return sysfs_emit(buf, "%u\n", ctx->policy.pipe_gen);
}
static ssize_t pipe_gen_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
    struct dw_pcie_pipe_phy *ctx = dev_get_drvdata(dev);
    unsigned long v;
    if (!allow_writes)
        return -EPERM;
    if (kstrtoul(buf, 0, &v))
        return -EINVAL;
    ctx->policy.pipe_gen = (u8)v;
    return count;
}
static DEVICE_ATTR_RW(pipe_gen);

static struct attribute *dw_attrs[] = {
    &dev_attr_refclk_hz.attr,
    &dev_attr_ssc_enable.attr,
    &dev_attr_pipe_gen.attr,
    NULL,
};
ATTRIBUTE_GROUPS(dw);

/* debugfs capability dump */
static int caps_show(struct seq_file *s, void *data)
{
    struct dw_pcie_pipe_phy *ctx = s->private;
    u8 tx, rx; bool garbage;
    if (dw_pcie_pipe_dump_caps(ctx, &tx, &rx, &garbage) == 0)
        seq_printf(s, "TX_MIN_WR=%u RX_WR=%u GARBAGE=%u\n", tx, rx, garbage ? 1 : 0);
    else
        seq_puts(s, "read failed\n");
    return 0;
}
DEFINE_SHOW_ATTRIBUTE(caps);

/* DT binding properties */
struct dw_pcie_dt_data {
    u32 refclk_hz;
    bool ssc_enable;
    u8 pipe_gen;
};

static int dw_pcie_pipe_probe(struct platform_device *pdev)
{
    struct dw_pcie_pipe_phy *ctx;
    struct resource *res;
    struct dw_pcie_dt_data d = { .refclk_hz = 100000000, .ssc_enable = true, .pipe_gen = 5 };

    ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;
    ctx->dev = &pdev->dev;

    of_property_read_u32(pdev->dev.of_node, "refclk-rate", &d.refclk_hz);
    of_property_read_u32(pdev->dev.of_node, "pipe-version", (u32 *)&d.pipe_gen);
    d.ssc_enable = of_property_read_bool(pdev->dev.of_node, "ssc");

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    ctx->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(ctx->base))
        return PTR_ERR(ctx->base);

    ctx->refclk = devm_clk_get_optional(&pdev->dev, "refclk");
    if (!IS_ERR(ctx->refclk))
        clk_prepare_enable(ctx->refclk);

    ctx->rst_pma = devm_reset_control_get_optional_exclusive(&pdev->dev, "pma");
    ctx->rst_pipe = devm_reset_control_get_optional_exclusive(&pdev->dev, "pipe");

    ctx->ext_phy = devm_phy_optional_get(&pdev->dev, "pcie-phy");
    if (IS_ERR(ctx->ext_phy))
        return PTR_ERR(ctx->ext_phy);
    if (ctx->ext_phy) {
        int r = phy_init(ctx->ext_phy);
        if (r)
            return r;
        r = phy_power_on(ctx->ext_phy);
        if (r)
            return r;
    }

    ctx->policy.prb_timeout_us = DW_PCIE_PRB_BUSY_TIMEOUT_US_DEFAULT;
    ctx->policy.refclk_hz = d.refclk_hz;
    ctx->policy.ssc_enable = d.ssc_enable;
    ctx->policy.pipe_gen = d.pipe_gen;

    /* Resets (optional) */
    if (ctx->rst_pma)
        reset_control_assert(ctx->rst_pma);
    if (ctx->rst_pipe)
        reset_control_assert(ctx->rst_pipe);

    udelay(10);

    if (ctx->rst_pma)
        reset_control_deassert(ctx->rst_pma);
    udelay(100);
    if (ctx->rst_pipe)
        reset_control_deassert(ctx->rst_pipe);

    platform_set_drvdata(pdev, ctx);

    /* sysfs */
    if (sysfs_create_groups(&pdev->dev.kobj, dw_groups))
        dev_warn(&pdev->dev, "failed to create sysfs groups\n");

    /* debugfs */
    if (!dbg_dir)
        dbg_dir = debugfs_create_dir("dw_pcie_pipe", NULL);
    if (!IS_ERR_OR_NULL(dbg_dir))
        debugfs_create_file("caps", 0400, dbg_dir, ctx, &caps_fops);

    dev_info(&pdev->dev, "DWC PCIe PIPE PHY shim probed. allow_writes=%d\n", allow_writes);
    return 0;
}

static int dw_pcie_pipe_remove(struct platform_device *pdev)
{
    struct dw_pcie_pipe_phy *ctx = platform_get_drvdata(pdev);

    sysfs_remove_groups(&pdev->dev.kobj, dw_groups);

    if (ctx->ext_phy) {
        phy_power_off(ctx->ext_phy);
        phy_exit(ctx->ext_phy);
    }
    if (!IS_ERR_OR_NULL(ctx->refclk))
        clk_disable_unprepare(ctx->refclk);

    return 0;
}

static const struct of_device_id dw_pcie_pipe_of_match[] = {
    { .compatible = "synopsys,dw-pcie-dm-pipe-phy" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, dw_pcie_pipe_of_match);

static struct platform_driver dw_pcie_pipe_driver = {
    .probe = dw_pcie_pipe_probe,
    .remove = dw_pcie_pipe_remove,
    .driver = {
        .name = "dw-pcie-dm-pipe-phy",
        .of_match_table = dw_pcie_pipe_of_match,
    },
};
module_platform_driver(dw_pcie_pipe_driver);

MODULE_DESCRIPTION("Synopsys DWC PCIe DM v6.00a External PIPE PHY integration shim");
MODULE_AUTHOR("RCI-ag-DrvGen Agent");
MODULE_LICENSE("BSD-3-Clause");
