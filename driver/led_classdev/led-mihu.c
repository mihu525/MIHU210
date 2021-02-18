/*************************************************************************
	> File Name: led-mihu.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月17日 星期日 16时46分13秒
 ************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
//#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/io.h>
//#include <linux/gpio.h>
//#include <linux/slab.h>

//#include <mach/hardware.h>
#include <mach/regs-gpio.h>
//#include <mach/leds-gpio.h>
//
#include <linux/module.h>


#define GPH0CON_VA S5PV210_GPH0_BASE
#define GPH0DAT_VA (S5PV210_GPH0_BASE + 4)





struct led_classdev      cdev;
 


static void s3c24xx_led_set(struct led_classdev *led_cdev,
                        enum led_brightness value)
{
    u32 reg;
    reg = readl((u32 *)GPH0CON_VA);
    reg |= (1<<12)|(1<<8)|(1<<4)|(1<<0);
    writel(reg,(u32 *)GPH0CON_VA);

    reg = readl((u32 *)GPH0DAT_VA);
    
    if(value == LED_OFF)
    {
        reg &= ~0xF;
    }
    else
    {
        reg |= 0xF;
    }
    writel(reg,(u32 *)GPH0DAT_VA);
}






static int __init s3c24xx_led_init(void)
{
    int ret = -1;
    
    cdev.name = "led-mihu";
    cdev.brightness = 0;
    cdev.brightness_set = s3c24xx_led_set;
    cdev.flags = 0;



    ret = led_classdev_register(NULL, &cdev);
    if(ret < 0)
    {
        printk(KERN_ERR"failed to classdev_register\n");
        return -1;
    }

    return 0;
    
}

static void __exit s3c24xx_led_exit(void)
{
    led_classdev_unregister(&cdev);   
}

module_init(s3c24xx_led_init);
module_exit(s3c24xx_led_exit);

//MODULE_AUTHOR("mihu525");
MODULE_DESCRIPTION("MIHU LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("mihu led for ledclass");

