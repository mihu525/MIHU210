/*
 *  drivers/input/misc/led_misc.c
 *
 *   mihu525  test
 *
*/

#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <mach/regs-gpio.h>
#include <linux/gpio.h>

#define NAME4		"led4"



#define BUFFER_SIZE			16

#define IOCTRL_OPEN  1
#define IOCTRL_CLOSE  0

MODULE_AUTHOR("mihu525");
MODULE_DESCRIPTION("Led use misc input driver");
MODULE_LICENSE("GPL");


static DEFINE_MUTEX(led_mutex);	/* Used to walk the hash */


#define LED1 S5PV210_GPH0(0) 
#define LED2 S5PV210_GPH0(1) 
#define LED3 S5PV210_GPH0(2) 
#define LED4 S5PV210_GPH0(3) 

static int open(struct inode *inode, struct file *file)
{
	int ret = -1;
	if(!mutex_trylock(&led_mutex)) // 不会用
		return -EBUSY;
	
	ret = gpio_request(LED4,NAME4);
	if(ret)
	{
		printk(KERN_ERR"failed to gpio_request");
        return -1;
	}
	else
	{
		gpio_direction_output(LED4,0);
	}
	
	return 0;

}

static int release(struct inode *inode, struct file *file)
{
	gpio_free(LED4);
	return 0;
}
static int ioctl (struct inode * inode, struct file *file,unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO"------ioctl----------");
	mutex_lock(&led_mutex);
	printk(KERN_INFO"-------lock---------");
	switch(cmd)
	{
        case IOCTRL_OPEN:
            gpio_set_value(LED4,1);
		break;
		default:	
            gpio_set_value(LED4,0);
		break;
	}
	mutex_unlock(&led_mutex);
	return 0;
}
static const struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= open,
	.ioctl		= ioctl,
	.release		= release,

};

static struct miscdevice led_misc = {
	.fops		= &fops,
	.name		= NAME4,
	.minor		= MISC_DYNAMIC_MINOR,
};

static int __init led_init(void)
{
	return misc_register(&led_misc);
}

static void __exit led_exit(void)
{
	misc_deregister(&led_misc);
}

module_init(led_init);
module_exit(led_exit);
