#include "linux/gfp_types.h"
#include "linux/wait.h"
#include "linux/workqueue.h"
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/slab.h>

static int num = 5;

module_param(num, int, S_IRUGO);

struct work_data {
    struct work_struct my_work;
    int test_data;
};

static struct workqueue_struct *my_workqueue;

void
my_work_func(struct work_struct *work) {
    struct work_data *my_data = container_of(work, struct work_data, my_work);
    pr_info("enter %s, %d\n", __FUNCTION__, __LINE__);
    pr_info("recv data: %d\n", my_data->test_data);
    kfree(my_data);
}

static int __init
hello_init(void) {
    struct work_data *my_data;
    pr_info("Hello World init \n");
    pr_info("paramter num = %d \n", num);
    my_workqueue = create_singlethread_workqueue("my_workqueue");
    my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    my_data->test_data = 0xdeaf;
    INIT_WORK(&my_data->my_work, my_work_func);
    queue_work(my_workqueue, &my_data->my_work);
    return 0;
}

static void __exit
hello_exit(void) {
    flush_workqueue(my_workqueue);
    destroy_workqueue(my_workqueue);
    pr_info("Hello World exit \n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("dansmiledan");

MODULE_DESCRIPTION("This is a print out HelloWorld moudle");
