/*************************************************************************
	> File Name: hello_world.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月04日 星期一 13时35分11秒
 ************************************************************************/

#include <linux/module.h>
#include <linux/init.h>

static int __init init(void)
{
    printk(KERN_DEBUG"hello world,I'm mihu\n");
    return 0;
}



static void __exit exit(void)
{
    printk(KERN_DEBUG"I'm go back\n");
}

module_init(init);
module_exit(exit);

MODULE_LICENSE("GPL");




