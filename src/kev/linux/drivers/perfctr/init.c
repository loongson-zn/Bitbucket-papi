/* $Id$
 * Performance-monitoring counters driver.
 * Top-level initialisation code.
 *
 * Copyright (C) 1999-2004  Mikael Pettersson
 */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/perfctr.h>

#include <asm/uaccess.h>

#include "compat.h"
#include "virtual.h"
#include "global.h"
#include "version.h"
#include "marshal.h"

MODULE_AUTHOR("Mikael Pettersson <mikpe@csd.uu.se>");
MODULE_DESCRIPTION("Performance-monitoring counters driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("char-major-10-182");
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,63)
EXPORT_NO_SYMBOLS;
#endif

#ifdef CONFIG_PERFCTR_DEBUG
#define VERSION_DEBUG " DEBUG"
#else
#define VERSION_DEBUG
#endif

struct perfctr_info perfctr_info = {
	.abi_version = PERFCTR_ABI_VERSION,
	.driver_version = VERSION VERSION_DEBUG,
};

char *perfctr_cpu_name __initdata;

int sys_perfctr_abi(unsigned int *argp)
{
	if( put_user(PERFCTR_ABI_VERSION, argp) )
		return -EFAULT;
	return 0;
}

int sys_perfctr_info(struct perfctr_struct_buf *argp)
{
	return perfctr_copy_to_user(argp, &perfctr_info, &perfctr_info_sdesc);
}

static int cpus_copy_to_user(const cpumask_t *cpus, struct perfctr_cpu_mask *argp)
{
	const unsigned int k_nrwords = PERFCTR_CPUMASK_NRLONGS*(sizeof(long)/sizeof(int));
	unsigned int u_nrwords;
	unsigned int ui, ki, j;

	if( get_user(u_nrwords, &argp->nrwords) )
		return -EFAULT;
	if( put_user(k_nrwords, &argp->nrwords) )
		return -EFAULT;
	if( u_nrwords < k_nrwords )
		return -EOVERFLOW;
	for(ui = 0, ki = 0; ki < PERFCTR_CPUMASK_NRLONGS; ++ki) {
		unsigned long mask = cpus_addr(*cpus)[ki];
		for(j = 0; j < sizeof(long)/sizeof(int); ++j) {
			if( put_user((unsigned int)mask, &argp->mask[ui]) )
				return -EFAULT;
			++ui;
			mask = (mask >> (8*sizeof(int)-1)) >> 1;
		}
	}
	return 0;
}

int sys_perfctr_cpus(struct perfctr_cpu_mask *argp)
{
	cpumask_t cpus = cpu_online_map;
	return cpus_copy_to_user(&cpus, argp);
}

int sys_perfctr_cpus_forbidden(struct perfctr_cpu_mask *argp)
{
	cpumask_t cpus = perfctr_cpus_forbidden_mask;
	return cpus_copy_to_user(&cpus, argp);
}

#ifdef CONFIG_IA32_EMULATION
#include <asm/ioctl32.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,23)
static int perfctr_ioctl32_handler(unsigned int fd, unsigned int cmd, unsigned long arg, struct file *filp)
{
	/* filp->f_op->ioctl is known to exist; see sys32_ioctl() */
	return filp->f_op->ioctl(filp->f_dentry->d_inode, filp, cmd, arg);
}
#else
#define perfctr_ioctl32_handler	0
#endif

static void __init perfctr_register_ioctl32_conversions(void)
{
	int err;

	err  = register_ioctl32_conversion(PERFCTR_ABI, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(PERFCTR_INFO, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(PERFCTR_CPUS, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(PERFCTR_CPUS_FORBIDDEN, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_CREAT, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_OPEN, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_READ_SUM, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_UNLINK, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_CONTROL, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_IRESUME, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(VPERFCTR_READ_CONTROL, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(GPERFCTR_CONTROL, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(GPERFCTR_READ, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(GPERFCTR_STOP, perfctr_ioctl32_handler);
	err |= register_ioctl32_conversion(GPERFCTR_START, perfctr_ioctl32_handler);
	if( err )
		printk(KERN_ERR "perfctr: register_ioctl32_conversion() failed\n");
}

static void __exit perfctr_unregister_ioctl32_conversions(void)
{
	unregister_ioctl32_conversion(PERFCTR_ABI);
	unregister_ioctl32_conversion(PERFCTR_INFO);
	unregister_ioctl32_conversion(PERFCTR_CPUS);
	unregister_ioctl32_conversion(PERFCTR_CPUS_FORBIDDEN);
	unregister_ioctl32_conversion(VPERFCTR_CREAT);
	unregister_ioctl32_conversion(VPERFCTR_OPEN);
	unregister_ioctl32_conversion(VPERFCTR_READ_SUM);
	unregister_ioctl32_conversion(VPERFCTR_UNLINK);
	unregister_ioctl32_conversion(VPERFCTR_CONTROL);
	unregister_ioctl32_conversion(VPERFCTR_IRESUME);
	unregister_ioctl32_conversion(VPERFCTR_READ_CONTROL);
	unregister_ioctl32_conversion(GPERFCTR_CONTROL);
	unregister_ioctl32_conversion(GPERFCTR_READ);
	unregister_ioctl32_conversion(GPERFCTR_STOP);
	unregister_ioctl32_conversion(GPERFCTR_START);
}

#else
#define perfctr_register_ioctl32_conversions()		do{}while(0)
#define perfctr_unregister_ioctl32_conversions()	do{}while(0)
#endif

static int dev_perfctr_ioctl(struct inode *inode, struct file *filp,
			     unsigned int cmd, unsigned long arg)
{
	switch( cmd ) {
	case PERFCTR_ABI:
		return sys_perfctr_abi((unsigned int*)arg);
	case PERFCTR_INFO:
		return sys_perfctr_info((struct perfctr_struct_buf*)arg);
	case PERFCTR_CPUS:
		return sys_perfctr_cpus((struct perfctr_cpu_mask*)arg);
	case PERFCTR_CPUS_FORBIDDEN:
		return sys_perfctr_cpus_forbidden((struct perfctr_cpu_mask*)arg);
	case VPERFCTR_CREAT:
		return vperfctr_attach((int)arg, 1);
	case VPERFCTR_OPEN:
		return vperfctr_attach((int)arg, 0);
	default:
		return gperfctr_ioctl(inode, filp, cmd, arg);
	}
	return -EINVAL;
}

static struct file_operations dev_perfctr_file_ops = {
	.owner = THIS_MODULE,
	.ioctl = dev_perfctr_ioctl,
};

static struct miscdevice dev_perfctr = {
	.minor = 182,
	.name = "perfctr",
	.fops = &dev_perfctr_file_ops,
};

int __init perfctr_init(void)
{
	int err;
	if( (err = perfctr_cpu_init()) != 0 ) {
		printk(KERN_INFO "perfctr: not supported by this processor\n");
		return err;
	}
	if( (err = vperfctr_init()) != 0 )
		return err;
	gperfctr_init();
	if( (err = misc_register(&dev_perfctr)) != 0 ) {
		printk(KERN_ERR "/dev/perfctr: failed to register, errno %d\n",
		       -err);
		return err;
	}
	perfctr_register_ioctl32_conversions();
	printk(KERN_INFO "perfctr: driver %s, cpu type %s at %u kHz\n",
	       perfctr_info.driver_version,
	       perfctr_cpu_name,
	       perfctr_info.cpu_khz);
	return 0;
}

void __exit perfctr_exit(void)
{
	perfctr_unregister_ioctl32_conversions();
	misc_deregister(&dev_perfctr);
	vperfctr_exit();
	perfctr_cpu_exit();
}

module_init(perfctr_init)
module_exit(perfctr_exit)
