#include "linux/interrupt.h"
#include "linux/wait.h"
#include <linux/delay.h>
#include <linux/module.h>

static int num = 5;

module_param(num, int, S_IRUGO);

char *tasklet_test_data = "string for test";

void
my_tasklet_handler(struct tasklet_struct *task) {
    pr_info("%s: data: %s\n", __FUNCTION__, (char *) task->data);
}

DECLARE_TASKLET(my_tasklet, my_tasklet_handler);

static int __init
hello_init(void) {
    pr_info("enter %s\n", __FUNCTION__);
    my_tasklet.data = (unsigned long) tasklet_test_data;
    tasklet_schedule(&my_tasklet);
    return 0;
}

static void __exit
hello_exit(void) {
    pr_info("Hello World exit \n");
    tasklet_kill(&my_tasklet);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("dansmiledan");

MODULE_DESCRIPTION("This is a print out HelloWorld moudle");
