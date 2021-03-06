#include <linux/ioctl.h>

/* Use 'x' as magic number */
#define SCULL_IOC_MAGIC 'x'

#define SCULL_IOCRESET    _IO(SCULL_IOC_MAGIC, 0)

#define SCULL_IOCSQUANTUM _IOW(SCULL_IOC_MAGIC, 1, int);
#define SCULL_IOCSQSET    _IOW(SCULL_IOC_MAGIC, 2, int);
#define SCULL_IOCTQUANTUM _IO(SCULL_IOC_MAGIC, 3);
#define SCULL_IOCTQSET    _IO(SCULL_IOC_MAGIC, 4);
#define SCULL_IOCGQUANUM  _IOR(SCULL_IOC_MAGIC, 5, int);
#define SCULL_IOCGQSET    _IOR(SCULL_IOC_MAGIC, 6, int);
#define SCULL_IOCQQUANTUM _IO(SCULL_IOC_MAGIC, 7);
#define SCULL_IOCQQSET    _IO(SCULL_IOC_MAGIC, 8);
#define SCULL_IOXQUANTUM  _IOWR(SCULL_IOC_MAGIC, 9, int);
#define SCULL_IOXQSET     _IOWR(SCULL_IOC_MAGIC, 10, int);
#define SCULL_IOHQUANTUM  _IO(SCULL_IOC_MAGIC, 11);
#define SCULL_IOHQSET     _IO(SCULL_IOC_MAGIC, 12);

#define SCULL_IOC_MAGNR 14