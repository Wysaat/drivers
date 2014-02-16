static atomic_t scull_s_available = ATOMIC_INIT(1);

static scull_s_open(struct inode *inode, struct file *filp) {
	struct scull_dev *dev = &scull_s_device;

	if (!atomic_dec_and_test(&scull_s_available)) {
		atomic_inc(&scull_s_available);
		return -EBUSY;
	}

	if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
		scull_trim(dev);
	filp->private_data = dev;
	return 0;
}

static int scull_s_release(struct inode *inode, struct file *filp) {
	atomic_inc(&scull_s_available);
	return 0;
}