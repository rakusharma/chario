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

MODULE_LICENSE("GPL");

struct chario {
	int major;
	struct cdev cdev; 
	struct class *class;
	struct device *device;
};

#define DRV_NAME "chario"
#define CLASS_NAME "chariocl"

struct chario chario;

int chario_open(struct inode *node, struct file* fi)
{

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

static struct file_operations chario_fops = {
	.owner = THIS_MODULE,
	.open = chario_open,
	.release = chario_close,
	.read = chario_read,
	.write = chario_write

};

static int __init chario_init(void)
{
	dev_t dev_id;
	int rc;

	if((rc = alloc_chrdev_region(&dev_id, 0, 1, DRV_NAME))) {
		pr_err("Unable to register chario\n");
		return rc;
	}

	chario.major = MAJOR(dev_id);
	
	chario.class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(chario.class)) {
		unregister_chrdev_region(MKDEV(chario.major, 0), 1);
		return PTR_ERR(chario.class);
	}

	chario.device = device_create(chario.class, NULL, dev_id, NULL, CLASS_NAME);

	if(IS_ERR(chario.device)) {
		class_destroy(chario.class);
		unregister_chrdev_region(MKDEV(chario.major, 0), 1);
		return PTR_ERR(chario.device);
	}

	cdev_init(&chario.cdev, &chario_fops);

	rc = cdev_add(&chario.cdev, chario.major, 1);

	if (rc) {
		device_destroy(chario.class, 1);
		class_destroy(chario.class);
		unregister_chrdev_region(MKDEV(chario.major, 0), 1);
		return 0;
	}


	return 0;
}

static void __exit chario_exit(void)
{

	device_destroy(chario.class, 1);
	class_destroy(chario.class);
	unregister_chrdev_region(MKDEV(chario.major, 0), 1);

}

module_init(chario_init);
module_exit(chario_exit);

