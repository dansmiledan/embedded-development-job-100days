#include <linux/module.h>

static int num = 5;

module_param(num, int, S_IRUGO);

static int __init
hello_init(void) {
    pr_info("Hello World init \n");
    pr_info("paramter num = %d \n", num);
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
