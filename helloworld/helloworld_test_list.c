#include "linux/gfp_types.h"
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

static int num = 5;

module_param(num, int, S_IRUGO);

struct car {
    int count;
    struct list_head list;
};

static int __init
hello_init(void) {
    LIST_HEAD(carlist);
    struct car *tmpcar;
    struct car *redcar = kmalloc(sizeof(struct car), GFP_KERNEL);
    struct car *bluecar = kmalloc(sizeof(struct car), GFP_KERNEL);
    pr_info("Hello World init \n");
    pr_info("paramter num = %d \n", num);
    redcar->count = 1;
    bluecar->count = 2;
    list_add(&(redcar->list), &carlist);
    list_add(&(bluecar->list), &carlist);
    list_for_each_entry(tmpcar, &carlist, list) { pr_info("cat count %d", tmpcar->count); }
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
