/*
 *	@file: chario.c
 *	@author: Ravi Sharma
 *	@date: 4 june 2018
 *
 * 
 * */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");

struct chario {
	int major;
	struct cdev cdev; 
	struct class *class;
	struct device *device;
};

/*/dev*/
#define DEVICE_NAME "chariod"
/*/sys/classes*/
#define CLASS_NAME "chariocl"

/*main data type*/
struct chario chario;

int chario_open(struct inode *node, struct file* fi)
{
	struct chario *chario; 

	chario = container_of(node->i_cdev, struct chario, cdev);

	fi->private_data = chario;

	printk("file gets opened\n");


	return 0;
}

int chario_close(struct inode* node, struct file* fi)
{

	return 0;
}

ssize_t chario_read(struct file *fi, char __user *us, size_t si, loff_t *loff)
{

	return 0;
}

ssize_t chario_write(struct file *fi, const char __user *us, size_t si, loff_t *loff)
{

	return 0;
}

#define CHARIO_GET_REG_IOCTL _IOR(0xCC, 0x01, unsigned int)
#define CHARIO_SET_REG_IOCTL _IOW(0xCC, 0x02, unsigned int)

long chario_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct chario *chario = filp->private_data;
	int ret;

	printk("%s command %4d\n", __func__, cmd);

	switch(cmd) {
		case CHARIO_GET_REG_IOCTL:
			ret = __put_user(0x11, (int __user *)arg);
			return ret;
			break;
		case CHARIO_SET_REG_IOCTL:
			chario = chario;
			return 0;
			break;
		default:
			break;

	}


	return -EINVAL;

}

/*chario file operations*/
static struct file_operations chario_fops = {
	.owner = THIS_MODULE,
	.open = chario_open,
	.unlocked_ioctl = chario_ioctl,
	.read = chario_read,
	.write = chario_write,
	.release = chario_close,

};

static int __init chario_init(void)
{

	/*major number*/
	chario.major = register_chrdev(0, DEVICE_NAME, &chario_fops);
	if (( chario.major < 0 )) {
		pr_err("Unable to register chario\n");
		return chario.major;
	}

	printk("Registered with major %4d\n",chario.major);


	/*class & device creation leads to /sys/classes and /dev*/
	/*ldd shows script method which is not needed if we use this*/
	chario.class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(chario.class)) {
		printk("class create failed\n");
		unregister_chrdev(chario.major, DEVICE_NAME);
		return PTR_ERR(chario.class);
	}

	chario.device = device_create(chario.class, NULL, MKDEV(chario.major, 0), NULL, DEVICE_NAME);
	if(IS_ERR(chario.device)) {
		printk("device create failed\n");
		class_destroy(chario.class);
		unregister_chrdev(chario.major, DEVICE_NAME);
		return PTR_ERR(chario.device);
	}


	return 0;
}

static void __exit chario_exit(void)
{

	device_destroy(chario.class, MKDEV(chario.major, 0));
	class_unregister(chario.class);
	class_destroy(chario.class);
	unregister_chrdev(chario.major, DEVICE_NAME);

}



module_init(chario_init);
module_exit(chario_exit);

