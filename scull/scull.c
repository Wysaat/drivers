#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>

int scull_init(void) {
	dev_t dev;
	unsigned int scull_minor = 0;
	unsigned int scull_nr_devs = 10;

	if ((alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull")) < 0) {
		printk(KERN_WARNING "scull: can't get major ");
		return -1;
	}

	unsigned int scull_major = MAJOR(dev);
}

struct file_operations scull_fops {
	.owner = THIS_MODULE,
	// .llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	// .ioctl = scull_ioctl,
	.open = scull_open,
	.release = scull.release,
};

struct scull_dev {
	struct scull_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	// unsigned int access_key;
	// struct semaphore sem;
	struct cdev cdev;
};

struct scull_qset {
	void **data;
	struct scull_qset *next;
}

static void scull_setup_cdev(struct scull_dev *dev, int index) {
	int err, devno = MKDEV(scull_major, scull_minor + index);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

int scull_open(struct inode *inode, struct file *filp) {
	struct scull_dev *dev;

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;

	if ((filp->f_flags & O_ACCMODE) == O_WRONLY) {
		scull_trim(dev);
	}
	return 0;
}

int scull_release(struct inode *inode, struct file *filp) {
	return 0;
}

int scull_trim()