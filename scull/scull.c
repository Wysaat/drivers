#include <linux/types.h>
#include <linux/fs.h>


int scull_init(void) {
	dev_t dev;
	unsigned int scull_minor = 0;
	unsigned int scull_nr_devs = 10;

	if ((result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull")) < 0) {
		pirntk(KERN_WARNING "scull: can't get major ");
		return -1;
	}

	unsigned int scull_major = MAJOR(dev);
}