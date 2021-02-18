/*************************************************************************
	> File Name: hello_world.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月04日 星期一 13时35分11秒
 ************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


static int major_num = 0;
#define NAME "Mihu first chardev"



    
static int open (struct inode *inode, struct file *file)
{
    printk(KERN_DEBUG"I'm opened\n");
    return 0;
}
    
static int release (struct inode *inode, struct file *file)
{
    printk(KERN_DEBUG"I'm closed\n");
    return 0;
}


static const struct file_operations fops = {
    .owner      = THIS_MODULE,
    .open       = open,
    .release    = release,
};



static int __init init(void)
{
    printk(KERN_DEBUG"hello world,I'm mihu\n");
    if((major_num = register_chrdev(major_num, NAME, &fops))<0)
    {
        printk(KERN_ERR "%s: failed to create /dev entry.\n", NAME);
        return major_num;
    }

    printk(KERN_DEBUG"%s major num is %d.\n", NAME, major_num);

    return 0;
}

static void __exit exit(void)
{
    printk(KERN_DEBUG"I'm go back\n");
    unregister_chrdev(major_num, NAME);
}


module_init(init);
module_exit(exit);

MODULE_LICENSE("GPL");




