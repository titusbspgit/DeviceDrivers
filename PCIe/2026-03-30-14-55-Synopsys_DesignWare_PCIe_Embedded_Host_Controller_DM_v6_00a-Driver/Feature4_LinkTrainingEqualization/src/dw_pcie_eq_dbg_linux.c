// SPDX-License-Identifier: BSD-3-Clause
/* Linux kernel OOT debug driver wrapping the EQ HAL and exposing sysfs/debugfs */
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include "../include/dw_pcie_eq_dbg.h"

static bool allow_writes;
module_param(allow_writes, bool, 0644);
MODULE_PARM_DESC(allow_writes, "Enable write access to EQ controls (DANGEROUS)");

static unsigned int retrain_timeout_ms = 100;
module_param(retrain_timeout_ms, uint, 0644);
MODULE_PARM_DESC(retrain_timeout_ms, "Link retrain wait timeout (ms)");

static unsigned int eq_timeout_ms = 24;
module_param(eq_timeout_ms, uint, 0644);
MODULE_PARM_DESC(eq_timeout_ms, "EQ master phase timeout (ms)");

static char *bdf = (char *)"0000:00:00.0";
module_param(bdf, charp, 0444);
MODULE_PARM_DESC(bdf, "Target PCI device BDF domain:bus:dev.fn");

struct eqdbg_dev {
	struct pci_dev *pdev;
	void __iomem *dbi;
	dw_pcie_eq_ctx_t ctx;
	struct dentry *dbg_dir;
};

static uint32_t k_rd32(uintptr_t base, uint32_t off){ return readl((void __iomem *)(base + (uintptr_t)off)); }
static void k_wr32(uintptr_t base, uint32_t off, uint32_t v){ writel(v, (void __iomem *)(base + (uintptr_t)off)); }
static void k_udelay(uint32_t usec){ udelay(usec); }
static int k_get_link_training_dllla(void *user, bool *training, bool *dllla)
{
	struct pci_dev *pdev = (struct pci_dev *)user;
	u8 cap_ptr; u16 lnksts;
	if (!pdev || !training || !dllla) return -EINVAL;
	if (pci_read_config_byte(pdev, 0x34, &cap_ptr) != PCIBIOS_SUCCESSFUL) return -EIO;
	if (pci_read_config_word(pdev, (u16)cap_ptr + 0x12u, &lnksts) != PCIBIOS_SUCCESSFUL) return -EIO;
	*training = ((lnksts & (1u << 11)) != 0u);
	*dllla = ((lnksts & (1u << 13)) != 0u);
	return 0;
}

static const dw_pcie_eq_hal_ops_t k_ops = {
	.rd32 = k_rd32,
	.wr32 = k_wr32,
	.udelay = k_udelay,
	.get_link_training_dllla = k_get_link_training_dllla,
};

/* sysfs attributes under the PCI device directory */

static ssize_t gen3_related_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev);
	uint32_t v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_RELATED_OFF);
	return sysfs_emit(buf, "0x%08x\n", v);
}
static ssize_t gen3_related_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev); unsigned long v;
	if (!allow_writes) return -EPERM;
	if (kstrtoul(buf, 0, &v)) return -EINVAL;
	/* Protected write */
	uint32_t misc = readl(ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(misc | DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel((u32)v, ed->dbi + DWC_PCIE_DSP_GEN3_RELATED_OFF);
	writel(misc & ~DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	return count;
}
static DEVICE_ATTR_RW(gen3_related);

static ssize_t local_fs_lf_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev);
	uint32_t v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF);
	u8 lf = (u8)((v >> GEN3_EQ_LOCAL_LF_SHIFT) & GEN3_EQ_LOCAL_LF_MASK);
	u8 fs = (u8)((v >> GEN3_EQ_LOCAL_FS_SHIFT) & GEN3_EQ_LOCAL_FS_MASK);
	return sysfs_emit(buf, "FS=%u LF=%u (0x%08x)\n", fs, lf, v);
}
static ssize_t local_fs_lf_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev); unsigned long fs, lf; uint32_t v; int r;
	if (!allow_writes) return -EPERM;
	r = sscanf(buf, "%lu %lu", &fs, &lf);
	if (r != 2 || fs > 63 || lf > 63) return -EINVAL;
	v = dw_pcie_pack_fs_lf((u8)fs, (u8)lf);
	/* Protected write */
	uint32_t misc = readl(ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(misc | DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(v, ed->dbi + DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF);
	writel(misc & ~DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	return count;
}
static DEVICE_ATTR_RW(local_fs_lf);

static ssize_t coeff_legal_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev);
	uint32_t v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_COEFF_LEGAL_STATUS_OFF);
	return sysfs_emit(buf, "0x%08x\n", v);
}
static DEVICE_ATTR_RO(coeff_legal_status);

static ssize_t preset_index_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev); unsigned long idx;
	if (!allow_writes) return -EPERM;
	if (kstrtoul(buf, 0, &idx) || idx > 10) return -EINVAL;
	/* Protected write */
	uint32_t misc = readl(ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(misc | DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel((u32)idx, ed->dbi + DWC_PCIE_DSP_GEN3_EQ_PSET_INDEX_OFF);
	writel(misc & ~DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	return count;
}
static DEVICE_ATTR_WO(preset_index);

static ssize_t preset_tuple_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev);
	uint32_t v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF);
	u8 pre = (u8)((v >> GEN3_EQ_PSET_PRE_SHIFT) & GEN3_EQ_PSET_PRE_MASK);
	u8 cur = (u8)((v >> GEN3_EQ_PSET_CUR_SHIFT) & GEN3_EQ_PSET_CUR_MASK);
	u8 post= (u8)((v >> GEN3_EQ_PSET_POST_SHIFT) & GEN3_EQ_PSET_POST_MASK);
	return sysfs_emit(buf, "PRE=%u CUR=%u POST=%u (0x%08x)\n", pre, cur, post, v);
}
static ssize_t preset_tuple_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev); unsigned long pre, cur, post; int r; uint32_t v;
	if (!allow_writes) return -EPERM;
	r = sscanf(buf, "%lu %lu %lu", &pre, &cur, &post);
	if (r != 3 || pre > 63 || cur > 63 || post > 63) return -EINVAL;
	v = dw_pcie_pack_pset((u8)pre, (u8)cur, (u8)post);
	/* Protected write */
	uint32_t misc = readl(ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(misc | DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(v, ed->dbi + DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF);
	writel(misc & ~DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	return count;
}
static DEVICE_ATTR_RW(preset_tuple);

static ssize_t preset_vector_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev); unsigned long m;
	if (!allow_writes) return -EPERM;
	if (kstrtoul(buf, 0, &m)) return -EINVAL;
	/* RMW: update only PVEC */
	uint32_t v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF);
	uint32_t newv = (v & ~(GEN3_EQ_CTRL_PVEC_MASK << GEN3_EQ_CTRL_PVEC_SHIFT)) | (((u32)m & GEN3_EQ_CTRL_PVEC_MASK) << GEN3_EQ_CTRL_PVEC_SHIFT);
	/* Protected write */
	uint32_t misc = readl(ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(misc | DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	writel(newv, ed->dbi + DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF);
	writel(misc & ~DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN, ed->dbi + DWC_PCIE_MISC_CONTROL_1_OFF);
	return count;
}
static DEVICE_ATTR_WO(preset_vector);

static ssize_t force_recovery_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct eqdbg_dev *ed = dev_get_drvdata(dev); unsigned long one;
	if (!allow_writes) return -EPERM;
	if (kstrtoul(buf, 0, &one) || one == 0) return -EINVAL;
	/* Force recovery and wait for L0 */
	(void)dw_pcie_eq_force_recovery(&ed->ctx);
	ed->ctx.retrain_timeout_us = retrain_timeout_ms * 1000u;
	(void)dw_pcie_eq_enter_recovery_and_wait(&ed->ctx);
	return count;
}
static DEVICE_ATTR_WO(force_recovery);

/* Disable detect/disable via sysfs to avoid accidental link down; debugfs will provide explicit controls */

static struct attribute *eq_attrs[] = {
	&dev_attr_gen3_related.attr,
	&dev_attr_local_fs_lf.attr,
	&dev_attr_coeff_legal_status.attr,
	&dev_attr_preset_index.attr,
	&dev_attr_preset_tuple.attr,
	&dev_attr_preset_vector.attr,
	&dev_attr_force_recovery.attr,
	NULL,
};
static const struct attribute_group eq_attr_group = { .name = "eq_dbg", .attrs = eq_attrs };

/* debugfs dump: pset[0..10] and key registers */
static int dbg_summary_show(struct seq_file *s, void *data)
{
	struct eqdbg_dev *ed = s->private; int i; u32 v;
	seq_puts(s, "== EQ Registers ==\n");
	v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_RELATED_OFF);
	seq_printf(s, "GEN3_RELATED: 0x%08x\n", v);
	v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF);
	seq_printf(s, "LOCAL_FS_LF: 0x%08x (FS=%u LF=%u)\n", v,
		(u8)((v >> GEN3_EQ_LOCAL_FS_SHIFT) & GEN3_EQ_LOCAL_FS_MASK),
		(u8)((v >> GEN3_EQ_LOCAL_LF_SHIFT) & GEN3_EQ_LOCAL_LF_MASK));
	v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF);
	seq_printf(s, "EQ_CONTROL: 0x%08x\n", v);
	v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_COEFF_LEGAL_STATUS_OFF);
	seq_printf(s, "COEFF_LEGAL_STATUS: 0x%08x\n", v);
	for (i = 0; i <= 10; ++i) {
		/* index then read tuple */
		writel(i, ed->dbi + DWC_PCIE_DSP_GEN3_EQ_PSET_INDEX_OFF);
		v = readl(ed->dbi + DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF);
		seq_printf(s, "PSET[%d]=0x%08x (PRE=%u CUR=%u POST=%u)\n", i, v,
			(u8)((v >> GEN3_EQ_PSET_PRE_SHIFT) & GEN3_EQ_PSET_PRE_MASK),
			(u8)((v >> GEN3_EQ_PSET_CUR_SHIFT) & GEN3_EQ_PSET_CUR_MASK),
			(u8)((v >> GEN3_EQ_PSET_POST_SHIFT) & GEN3_EQ_PSET_POST_MASK));
	}
	return 0;
}
static int dbg_open(struct inode *inode, struct file *file){ return single_open(file, dbg_summary_show, inode->i_private);} 
static const struct file_operations dbg_fops = { .owner = THIS_MODULE, .open = dbg_open, .read = seq_read, .llseek = seq_lseek, .release = single_release };

static struct dentry *dbg_root;

static int eqdbg_create(struct pci_dev *pdev)
{
	int ret; struct eqdbg_dev *ed; resource_size_t start, len;
	ed = devm_kzalloc(&pdev->dev, sizeof(*ed), GFP_KERNEL);
	if (!ed) return -ENOMEM;
	ed->pdev = pdev;
	start = pci_resource_start(pdev, 0);
	len = pci_resource_len(pdev, 0);
	if (!start || !len) return -ENODEV;
	ed->dbi = devm_ioremap(&pdev->dev, start, len);
	if (!ed->dbi) return -ENOMEM;

	/* HAL ctx */
	if (dw_pcie_eq_init(&ed->ctx, (uintptr_t)ed->dbi, (void *)pdev, &k_ops) != 0) return -EINVAL;
	ed->ctx.retrain_timeout_us = retrain_timeout_ms * 1000u;
	ed->ctx.eq_timeout_us = eq_timeout_ms * 1000u;

	dev_set_drvdata(&pdev->dev, ed);
	ret = sysfs_create_group(&pdev->dev.kobj, &eq_attr_group);
	if (ret) return ret;

	if (!dbg_root) dbg_root = debugfs_create_dir("dw_pcie_eq", NULL);
	if (!IS_ERR_OR_NULL(dbg_root)) {
		char name[32]; snprintf(name, sizeof(name), "%s", pci_name(pdev));
		ed->dbg_dir = debugfs_create_dir(name, dbg_root);
		if (!IS_ERR_OR_NULL(ed->dbg_dir)) {
			debugfs_create_file("summary", 0400, ed->dbg_dir, ed, &dbg_fops);
		}
	}
	dev_info(&pdev->dev, "EQ debug: mapped DBI @%pa allow_writes=%d\n", &start, allow_writes);
	return 0;
}

static void eqdbg_destroy(struct pci_dev *pdev)
{
	struct eqdbg_dev *ed = dev_get_drvdata(&pdev->dev);
	if (!ed) return;
	sysfs_remove_group(&pdev->dev.kobj, &eq_attr_group);
	if (ed->dbg_dir) debugfs_remove_recursive(ed->dbg_dir);
	dev_set_drvdata(&pdev->dev, NULL);
}

static int parse_bdf(const char *s, int *domain, unsigned int *bus, unsigned int *dev, unsigned int *fn)
{ int d=0; unsigned int b=0,u=0,f=0; if (sscanf(s, "%x:%x:%x.%x", &d, &b, &u, &f) != 4) return -EINVAL; *domain=d; *bus=b; *dev=u; *fn=f; return 0; }

static struct pci_dev *g_pdev;

static int __init eqdbg_init(void)
{
	int d, err; unsigned int b, s, f; struct pci_dev *pdev;
	err = parse_bdf(bdf, &d, &b, &s, &f); if (err) { pr_err("eqdbg: invalid bdf %s\n", bdf); return -EINVAL; }
	pdev = pci_get_domain_bus_and_slot(d, b, PCI_DEVFN(s, f)); if (!pdev) { pr_err("eqdbg: PCI %s not found\n", bdf); return -ENODEV; }
	err = eqdbg_create(pdev);
	pci_dev_put(pdev);
	if (!err) g_pdev = pdev; /* keep pointer only for messaging; devm handles resources */
	return err;
}

static void __exit eqdbg_exit(void)
{
	if (g_pdev) eqdbg_destroy(g_pdev);
	if (dbg_root) { debugfs_remove_recursive(dbg_root); dbg_root = NULL; }
}

module_init(eqdbg_init);
module_exit(eqdbg_exit);

MODULE_DESCRIPTION("Synopsys DWC PCIe DM v6.00a - Link Training/EQ debug (sysfs/debugfs)");
MODULE_AUTHOR("RCI-ag-DrvGen Agent");
MODULE_LICENSE("BSD-3-Clause");
