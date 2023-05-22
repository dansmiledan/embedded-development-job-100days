#include "linux/container_of.h"
#include "linux/device.h"
#include "linux/device/class.h"
#include "linux/export.h"
#include "linux/fs.h"
#include "linux/kdev_t.h"
#include "linux/platform_device.h"
#include <linux/cdev.h>
#include <linux/module.h>

#define EEP_MAGIC        'E'
#define EEP_CMD_0_SEQ_NO 0x1
#define EEP_CMD_1_SEQ_NO 0x2

#define EEP_CMD_0 _IO(EEP_MAGIC, EEP_CMD_0_SEQ_NO)
#define EEP_CMD_1 _IOW(EEP_MAGIC, EEP_CMD_1_SEQ_NO, unsigned long)

#define EEP_TEST_BASE   1000
#define EEP_NBANK       8
#define EEP_DEVICE_NAME "eep-mem"
#define EEP_CLASS       "eep-class"

struct eep_data {
    struct cdev eep_cdev;
    int test_data;
};

struct class *eep_class;
struct eep_data eep_data[EEP_NBANK];
dev_t dev_num;

static int
my_open(struct inode *inode, struct file *filp) {
    struct eep_data *my_data = container_of(inode->i_cdev, struct eep_data, eep_cdev);
    pr_info("enter %s, get predefined test data %d\n", __FUNCTION__, my_data->test_data);
    return 0;
}

/*
 * free memeory
 * close device; drop buffer;
 */
static int
my_release(struct inode *inode, struct file *filp) {
    pr_info("enter %s\n", __FUNCTION__);
    return 0;
}

/*
 * 0 means EOF
 */
static ssize_t
my_read(struct file *filp, char __user *buf, size_t count, loff_t *pos) {
    pr_info("enter %s\n", __FUNCTION__);
    return 0;
}

/*
 * try:
 * memory size
 * address translation
 * data copy
 *
 */
static ssize_t
my_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos) {
    pr_info("enter %s\n", __FUNCTION__);
    return 0;
}

static loff_t
my_llseek(struct file *filp, loff_t offset, int whence) {
    pr_info("enter %s\n", __FUNCTION__);
    return 0;
}

static unsigned int
my_poll(struct file *filp, struct poll_table_struct *pt) {
    pr_info("enter %s\n", __FUNCTION__);
    return 0;
}

static long
my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    pr_info("enter %s\n", __FUNCTION__);
    switch (cmd) {
    case EEP_CMD_0:
        pr_info("recv cmd 0");
        break;
    case EEP_CMD_1:
        pr_info("recv cmd 1");
        break;
    default:
        return -ENOTTY;
    }
    return 0;
}

const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .write = my_write,
    .read = my_read,
    .llseek = my_llseek,
    .poll = my_poll,
    .compat_ioctl = my_ioctl,
};

static int __init
my_init(void) {
    int i;
    dev_t tmp_dev;
    alloc_chrdev_region(&dev_num, 0, EEP_NBANK, EEP_DEVICE_NAME);
    eep_class = class_create(THIS_MODULE, EEP_CLASS);
    for (i = 0; i < EEP_NBANK; i++) {
        cdev_init(&eep_data[i].eep_cdev, &fops);
        eep_data[i].eep_cdev.owner = THIS_MODULE;
        eep_data[i].eep_cdev.ops = &fops;
        eep_data[i].test_data = i + EEP_TEST_BASE;
        tmp_dev = MKDEV(MAJOR(dev_num), MINOR(dev_num) + i);

        cdev_add(&eep_data[i].eep_cdev, tmp_dev, 1);
        device_create(eep_class, NULL, tmp_dev, NULL, EEP_DEVICE_NAME "%d", i);
    }
    return 0;
}

static void __exit
my_exit(void) {
    int i;
    dev_t tmp_dev;
    pr_info("enter %s\n", __FUNCTION__);
    for (i = 0; i < EEP_NBANK; i++) {
        tmp_dev = MKDEV(MAJOR(dev_num), MINOR(dev_num) + i);
        device_destroy(eep_class, tmp_dev);
        cdev_del(&eep_data[i].eep_cdev);
    }
    class_destroy(eep_class);
    unregister_chrdev_region(dev_num, EEP_NBANK);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dansmiledan");

MODULE_DESCRIPTION("basic char device driver for memory operation");