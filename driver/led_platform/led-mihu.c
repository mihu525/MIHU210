/*************************************************************************
	> File Name: led-mihu.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月17日 星期日 16时46分13秒
 ************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/io.h>
//#include <linux/gpio.h>
#include <linux/slab.h>

//#include <mach/hardware.h>
#include <mach/regs-gpio.h>
//#include <mach/leds-gpio.h>
#include <mach/gpio.h>
#include <linux/module.h>
#include <mach/leds-gpio.h>


struct s5pv210_gpio_led {
	struct led_classdev		 cdev;
	struct s5pv210_led_platdata	*pdata;
};
 

static inline struct s5pv210_gpio_led *pdev_to_gpio(struct platform_device *dev)
{
	return platform_get_drvdata(dev);
}

static inline struct s5pv210_gpio_led *to_gpio(struct led_classdev *led_cdev)
{
	return container_of(led_cdev, struct s5pv210_gpio_led, cdev);
}

static void s5pv210_led_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	struct s5pv210_gpio_led *led = to_gpio(led_cdev);
	struct s5pv210_led_platdata *pd = led->pdata;

	/* there will be a short delay between setting the output and
	 * going from output to input when using tristate. */

	printk(KERN_INFO"---s5pv210_led_set--- %d\n",pd->gpio);
	if(value == LED_OFF)
    {
       // reg &= ~0xF;
       gpio_set_value(pd->gpio,0);
    }
    else
    {
        //reg |= 0xF;
        gpio_set_value(pd->gpio,1);
    }

}

static int s5pv210_led_remove(struct platform_device *dev)
{
    struct s5pv210_gpio_led *led = pdev_to_gpio(dev);
    struct s5pv210_led_platdata *pd = led->pdata;

    led_classdev_unregister(&led->cdev);
    gpio_free(pd->gpio);
    kfree(led);


	return 0;
}

static int s5pv210_led_probe(struct platform_device *dev)
{
	struct s5pv210_led_platdata *pdata = dev->dev.platform_data;
	struct s5pv210_gpio_led *led;
	int ret;
	printk(KERN_INFO"---s5pv210_led_probe---\n");

	led = kzalloc(sizeof(struct s5pv210_gpio_led), GFP_KERNEL);
	if (led == NULL) {
		dev_err(&dev->dev, "No memory for device\n");
		return -ENOMEM;
	}

	platform_set_drvdata(dev, led);
	
	led->cdev.name = pdata->name;
    led->cdev.brightness = 0;
    led->cdev.brightness_set = s5pv210_led_set;
    led->cdev.flags = 0;

	led->pdata = pdata;

    if (gpio_request(pdata->gpio, pdata->name)) 
    {
        printk(KERN_ERR "gpio_request failed\n");
    } 
    else 
    {
        gpio_direction_output(pdata->gpio, 0);
    }
    
	/* register our new led device */

	ret = led_classdev_register(&dev->dev, &led->cdev);
	if (ret < 0) {
		dev_err(&dev->dev, "led_classdev_register failed\n");
		kfree(led);
		return ret;
	}

	return 0;
}


static struct platform_driver s5pv210_led_driver = {
	.probe		= s5pv210_led_probe,
	.remove		= s5pv210_led_remove,
	.driver		= {
		.name		= "s5pv210_led",
		.owner		= THIS_MODULE,
	},
};

static int __init s5pv210_led_init(void)
{
	return platform_driver_register(&s5pv210_led_driver);
}

static void __exit s5pv210_led_exit(void)
{
    platform_driver_unregister(&s5pv210_led_driver);
}

module_init(s5pv210_led_init);
module_exit(s5pv210_led_exit);

MODULE_AUTHOR("mihu525 mihu525@qq.com");
MODULE_DESCRIPTION("MIHU LED driver in s5pv210");
MODULE_LICENSE("GPL");
MODULE_ALIAS("mihu led for ledclass");

