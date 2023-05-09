#include "linux/gfp_types.h"
#include "linux/jiffies.h"
#include "linux/timer.h"
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

static int num = 5;

module_param(num, int, S_IRUGO);

struct timer_list my_timer;

void
my_timer_handler(struct timer_list *timer) {
    pr_info("call %s\n", __FUNCTION__);
}

static int __init
hello_init(void) {
    int retval;
    pr_info("Hello World init \n");
    pr_info("paramter num = %d \n", num);
    timer_setup(&my_timer, my_timer_handler, 0);
    retval = mod_timer(&my_timer, jiffies + msecs_to_jiffies(3000));
    if (retval) {
        pr_info("fire timer failed!\n");
    }
    return 0;
}

static void __exit
hello_exit(void) {
    int retval;
    pr_info("Hello World exit \n");
    retval = del_timer(&my_timer);
    if (retval) {
        pr_info("the timer is still running!\n");
    }
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("dansmiledan");

MODULE_DESCRIPTION("This is a print out HelloWorld moudle");
