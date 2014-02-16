#include <linux/capability.h>
#include <linux/sched.h>

static int scull_u_open(struct inode *inode, struct file *filp) {
	struct scull_dev *dev = &scull_u_device;

	spin_lock(&scull_u_lock);
	if (scull_u_count &&
		    (scull_u_owner != current->uid) &&
		    (scull_u_owner != current->euid) &&
		    !capable(CAP_DAC_OVERRIDE)) {
		spin_unlock(&scull_u_lock);
	    return -EBUSY;
	}

	if (scull_u_count == 0)
		scull_u_owner = current->uid;

	scull_u_count++;
	spin_unlock(&scull_u_lock);

	if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
		scull_trim(dev);
	filp->private_data = dev;
	return 0;
}

static int scull_u_release(struct inode *inode, struct file *filp) {
	spin_lock(&scull_u_lock);
	scull_u_count--;
	spin_unlock(&scull_u_lock);
	return 0;
}

static inline int scull_w_available() {
	return scull_w_count == 0 ||
	    scull_w_owner == current_uid() ||
	    scull_w_owner == current_euid() ||
	    capable(CAP_DAC_OVERRIDE);
}

static int scull_w_open(struct inode *inode, struct file *filp) {
	struct scull_dev *dev = &scull_w_device;

	spin_lock(&scull_w_lock);
	while (!scull_w_available()) {
		spin_unlock(&scull_w_lock);
		if (filp->f_flags & O_NONBLOCK) return -EAGAIN;
		if (wait_event_interruptible(scull_w_wait, scull_w_available()))
			return -ERESTARTSYS;
		spin_lock(&scull_w_lock);
	}
	if (scull_count == 0)
		scull_owner = current->uid;
	scull_w_count++;
	spin_unlock(scull_w_lock);
}

static int scull_w_release(struct inode *inode, struct file *filp) {
	int temp;

	spin_lock(&scull_w_lock);
	scull_w_count--;
	temp = scull_w_count;
	spin_unlock(&scull_w_lock);

	if (temp == 0)
		wake_up_interruptible_sync(&scull_w_wait);
	return 0;
}