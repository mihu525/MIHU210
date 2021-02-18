/*************************************************************************
	> File Name: hello_world.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月04日 星期一 13时35分11秒
 ************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/ioport.h>

static int major_num = 0;
static char tmp[100] = {0};

#define NAME "Mihu first chardev"


#define GPH0CON 0xE0200C00u
#define GPH0DAT 0xE0200C04u
    
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

static ssize_t read(struct file *file, char __user *buf, size_t count,
                loff_t *offset)
{
    int ret;
    printk(KERN_DEBUG"led read\n");

    ret = copy_to_user(buf, tmp, count);
    if(ret){
        printk(KERN_DEBUG"copy to user last %d\n",ret);
        return -EFAULT;
    }


    return count;
    
}

static ssize_t write(struct file *file, const char __user *buf,
                size_t count, loff_t *offset)
{
    int ret;
    u32 *pCON = NULL;
    u32 *pDAT = NULL;
    ret = copy_from_user(tmp, buf, count);
    if(ret){
        printk(KERN_DEBUG"cp from user last %d\n",ret);
        return -EFAULT;
    }

    if (!request_mem_region(GPH0CON, 8, "mihu led")) {
        printk(KERN_DEBUG "Memory region busy\n");
         return -EBUSY;
                        
    }

    pCON = ioremap(GPH0CON,4);
    pDAT = ioremap(GPH0DAT,4);

    *pCON = (1<<12)|(1<<8)|(1<<4)|(1<<0);

    *pDAT = tmp[0]&0xF;
    printk(KERN_DEBUG"led is 0x%x",tmp[0]);

    iounmap(pCON);
    iounmap(pDAT);
    release_mem_region(GPH0CON,8);


    return 0;
}


static const struct file_operations fops = {
    .owner      = THIS_MODULE,
    .open       = open,
    .release    = release,
    .write      = write,
    .read       = read,
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




