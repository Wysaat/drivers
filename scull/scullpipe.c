#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h> /* it is automatically included by <linux/fs.h> */
#include <linux/cdev.h>
#include <asm/shed.h>
#include <linux/shed.h>

struct scull_pipe {
	wait_queue_head_t inq, outq;
	char *buffer, *end;
	int buffersize;
	char *rp, *wp;
	int nreaders, nwriters;
	struct fasync_struct *async_queue;
	struct semaphore sem;
	struct cdev cdev;
};

static ssize_t scull_p_read(struct file *filp, char __user *buf, size_t count, loff_t *fpos) {
	struct scull_pipe *dev = filp->private_data;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	while (dev->rp == dev->wp) {
		up(&dev->sem);
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		if (wait_event_interruptible(dev->inq, (dev->rp != dev->wp)))
			return -ERESTARTSYS;
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
	}

	if (dev->wp > dev->rp)
		count = min(count, (size_t)(dev->wp - dev->rp));
	else
		count = min(count, (size_t)(dev->end - dev->rp));
	if (copy_to_user(buf, dev->rp, count)) {
		up(&dev->sem);
		return -EFAULT;
	}
	dev->rp += count;
	if (dev->rp == dev->end)
}