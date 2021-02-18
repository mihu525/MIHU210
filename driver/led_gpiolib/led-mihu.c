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
#include <mach/gpio.h>
#include <linux/module.h>


#define GPH0CON_VA S5PV210_GPH0_BASE
#define GPH0DAT_VA (S5PV210_GPH0_BASE + 4)
#define NAME1 "mihu_led1"
#define NAME2 "mihu_led2"
#define NAME3 "mihu_led3"
#define NAME4 "mihu_led4"


#define LED1 S5PV210_GPH0(0)
#define LED2 S5PV210_GPH0(1)
#define LED3 S5PV210_GPH0(2)
#define LED4 S5PV210_GPH0(3)

struct led_classdev      cdev_led1;
struct led_classdev      cdev_led2;
struct led_classdev      cdev_led3;
struct led_classdev      cdev_led4;
 


static void s3c24xx_led4_set(struct led_classdev *led_cdev,
                        enum led_brightness value)
{
    u32 reg;
//    reg = readl((u32 *)GPH0CON_VA);
//    reg |= (1<<12)|(1<<8)|(1<<4)|(1<<0);
//    writel(reg,(u32 *)GPH0CON_VA);

//    reg = readl((u32 *)GPH0DAT_VA);
    
    if(value == LED_OFF)
    {
       // reg &= ~0xF;
       gpio_set_value(LED4,0);
    }
    else
    {
        //reg |= 0xF;
        gpio_set_value(LED4,1);
    }
    //writel(reg,(u32 *)GPH0DAT_VA);
}

static void s3c24xx_led1_set(struct led_classdev *led_cdev,
                        enum led_brightness value)
{
    u32 reg;
//    reg = readl((u32 *)GPH0CON_VA);
//    reg |= (1<<12)|(1<<8)|(1<<4)|(1<<0);
//    writel(reg,(u32 *)GPH0CON_VA);

//    reg = readl((u32 *)GPH0DAT_VA);
    
    if(value == LED_OFF)
    {
       // reg &= ~0xF;
       gpio_set_value(LED1,0);
    }
    else
    {
        //reg |= 0xF;
        gpio_set_value(LED1,1);
    }
    //writel(reg,(u32 *)GPH0DAT_VA);
}

static void s3c24xx_led2_set(struct led_classdev *led_cdev,
                        enum led_brightness value)
{
    u32 reg;
//    reg = readl((u32 *)GPH0CON_VA);
//    reg |= (1<<12)|(1<<8)|(1<<4)|(1<<0);
//    writel(reg,(u32 *)GPH0CON_VA);

//    reg = readl((u32 *)GPH0DAT_VA);
    
    if(value == LED_OFF)
    {
       // reg &= ~0xF;
       gpio_set_value(LED2,0);
    }
    else
    {
        //reg |= 0xF;
        gpio_set_value(LED2,1);
    }
    //writel(reg,(u32 *)GPH0DAT_VA);
}


static void s3c24xx_led3_set(struct led_classdev *led_cdev,
                        enum led_brightness value)
{
    u32 reg;
//    reg = readl((u32 *)GPH0CON_VA);
//    reg |= (1<<12)|(1<<8)|(1<<4)|(1<<0);
//    writel(reg,(u32 *)GPH0CON_VA);

//    reg = readl((u32 *)GPH0DAT_VA);
    
    if(value == LED_OFF)
    {
       // reg &= ~0xF;
       gpio_set_value(LED3,0);
    }
    else
    {
        //reg |= 0xF;
        gpio_set_value(LED3,1);
    }
    //writel(reg,(u32 *)GPH0DAT_VA);
}


static int __init s3c24xx_led_init(void)
{
    int ret = -1;
    
    ret = gpio_request(LED1, NAME1);
    ret |= gpio_request(LED2, NAME2);
    ret |= gpio_request(LED3, NAME3);
    ret |= gpio_request(LED4, NAME4);
    
    if(ret)
    {
        printk(KERN_ERR"gpio_reguest error\n");
        return -1;
    }

    ret = gpio_direction_output(LED1, 0);
    ret |= gpio_direction_output(LED2, 0);
    ret |= gpio_direction_output(LED3, 0);
    ret |= gpio_direction_output(LED4, 0);
    if(ret)
    {
        printk(KERN_ERR"direction_output error\n");
        goto unreg;
    }

    cdev_led1.name = "led1-mihu";
    cdev_led1.brightness = 0;
    cdev_led1.brightness_set = s3c24xx_led1_set;
    cdev_led1.flags = 0;

    cdev_led2.name = "led2-mihu";
    cdev_led2.brightness = 0;
    cdev_led2.brightness_set = s3c24xx_led2_set;
    cdev_led2.flags = 0;

    cdev_led3.name = "led3-mihu";
    cdev_led3.brightness = 0;
    cdev_led3.brightness_set = s3c24xx_led3_set;
    cdev_led3.flags = 0;

    cdev_led4.name = "led4-mihu";
    cdev_led4.brightness = 0;
    cdev_led4.brightness_set = s3c24xx_led4_set;
    cdev_led4.flags = 0;

    ret = led_classdev_register(NULL, &cdev_led1);
    ret |= led_classdev_register(NULL, &cdev_led2);
    ret |= led_classdev_register(NULL, &cdev_led3);
    ret |= led_classdev_register(NULL, &cdev_led4);

    if(ret < 0)
    {
        printk(KERN_ERR"failed to classdev_register\n");
        goto unreg;
    }
    
    return 0;

unreg:
    gpio_free(LED1);
    gpio_free(LED2);
    gpio_free(LED3);
    gpio_free(LED4);
    return -1;

    
}

static void __exit s3c24xx_led_exit(void)
{
    led_classdev_unregister(&cdev_led1);
    led_classdev_unregister(&cdev_led2);
    led_classdev_unregister(&cdev_led3);
    led_classdev_unregister(&cdev_led4);
    gpio_free(LED1);
    gpio_free(LED2);
    gpio_free(LED3);
    gpio_free(LED4);
}

module_init(s3c24xx_led_init);
module_exit(s3c24xx_led_exit);

MODULE_AUTHOR("mihu525 mihu525@qq.com");
MODULE_DESCRIPTION("MIHU LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("mihu led for ledclass");

