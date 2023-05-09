#include "linux/wait.h"
#include <linux/delay.h>
#include <linux/module.h>

static int num = 5;

module_param(num, int, S_IRUGO);

static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int condition = 0;
static struct work_struct my_work;

void
my_work_func(struct work_struct *work) {
    pr_info("enter %s, %d\n", __FUNCTION__, __LINE__);
    msleep(2000);
    pr_info("wake up when false condition\n");
    wake_up_interruptible(&my_wq);
    pr_info("sleep again\n");
    msleep(2000);
    condition = 1;
    pr_info("wake up when true condition\n");
    wake_up_interruptible(&my_wq);
}

static int __init
hello_init(void) {
    pr_info("Hello World init \n");
    pr_info("paramter num = %d \n", num);
    INIT_WORK(&my_work, my_work_func);
    schedule_work(&my_work);
    pr_info("Going to sleep \n");
    wait_event_interruptible(my_wq, condition != 0);
    pr_info("Wake up in %s\n", __FUNCTION__);
    return 0;
}

static void __exit
hello_exit(void) {
    pr_info("Hello World exit \n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("dansmiledan");

MODULE_DESCRIPTION("This is a print out HelloWorld moudle");
