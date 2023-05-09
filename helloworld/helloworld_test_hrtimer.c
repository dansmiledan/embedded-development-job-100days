#include "linux/gfp_types.h"
#include "linux/hrtimer.h"
#include "linux/jiffies.h"
#include "linux/timer.h"
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

static int num = 5;

module_param(num, int, S_IRUGO);

struct hrtimer my_hrtimer;

enum hrtimer_restart
my_hrtimer_callback(struct hrtimer *my_t) {
    pr_info("call %s\n", __FUNCTION__);
    return HRTIMER_NORESTART;
}

static int __init
hello_init(void) {
    pr_info("Hello World init \n");
    pr_info("paramter num = %d \n", num);
    hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_hrtimer.function = my_hrtimer_callback;
    hrtimer_set_expires(&my_hrtimer, ms_to_ktime(3000));
    hrtimer_start(&my_hrtimer, ms_to_ktime(3000), HRTIMER_MODE_REL);
    return 0;
}

static void __exit
hello_exit(void) {
    pr_info("Hello World exit \n");
    hrtimer_cancel(&my_hrtimer);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("dansmiledan");

MODULE_DESCRIPTION("This is a print out HelloWorld moudle");
