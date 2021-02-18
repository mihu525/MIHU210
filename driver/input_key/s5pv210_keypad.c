

#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>

#include <asm/irq.h>
#include <asm/io.h>

#include <linux/clk.h>
#include <mach/regs-clock.h>
#include <mach/map.h>
#include <mach/hardware.h>
#include <plat/regs-keypad.h>
#include <linux/interrupt.h>
#include "s3c-keypad.h"



static struct input_dev *button_dev;

//QT210 只有ROW0 和 ROW1
#define R_INT_MASK 0x30000
#define P_INT_MASK 0x0003
#define COL_MASK	0xF8
#define ROW_NR 2
#define COL_NR 5

#define BUTTON_IRQ IRQ_KEYPAD
#define REG_PA_KEYPAD  S3C_PA_KEYPAD
#define REG_SZ_KEYPAD  S3C_SZ_KEYPAD

static irqreturn_t button_interrupt(int irq, void *dummy)
{

    union {
        unsigned int reg;
        unsigned short r1p0[2];
    } row;
	int x = 0 ,y = 0; 
	unsigned char col = 0;
	static unsigned int code = 0;
	
	
	row.reg = readl(key_base + S3C_KEYIFSTSCLR) ;
    printk(KERN_INFO"row = 0x%x\n",row.reg);	
	if(row.r1p0[0]){
		
		for(x=0; x<ROW_NR; x++){
			if(row.r1p0[0] & (0x1<<x) ){
				printk(KERN_INFO"r:col = 0x%x",col);
				for(y=3; y<3+COL_NR; y++){
 				
                        unsigned int val;
                        val = KEYCOL_DMASK & ~(1 << x);
                        writel(code, key_base + S3C_KEYIFCOL);
                        val = ~(readl(key_base + S3C_KEYIFROW)) & ((1<<KEYPAD_ROWS) - 1);
                        if(val &(1<<x)){
                            code = keypad_keycode[(x<<3)+y];
                            printk(KERN_INFO"press:x=%d,y=%d,kepad=%d\n",x,y,code);
 						    input_report_key(button_dev, code, 1);
						    input_sync(button_dev);
                        }
					
				}
			}
		}
	}
	

	if(row.r1p0[1]){
        input_report_key(button_dev, code, 0);
        input_sync(button_dev);
       /* 
		for(x=0; x<ROW_NR; x++){
			if(row.r1p0[1] & (0x1<<x) ){
  				col = readl(key_base + S3C_KEYIFCOL);
				printk(KERN_INFO"r:col = 0x%x",col);
				for(y=3; y<3+COL_NR; y++){
 					if(col & (0x1<<y))
					{
                        unsigned int code ;
                        code = keypad_keycode[(x<<3)+y];
                        printk(KERN_INFO"relese:x=%d,y=%d,kepad=%d\n",x,y,code);
 						input_report_key(button_dev, code, 0);
						input_sync(button_dev);
					}
				}
			}
		}
        */
	}
		
	
	/*Clear the keypad interrupt status*/
	writel(KEYIFSTSCLR_CLEAR, key_base +S3C_KEYIFSTSCLR);

	return IRQ_HANDLED;
}

static int __init button_init(void)
{
	int error = 0;
	int key, code;
	static struct clk *keypad_clock;
    struct resource *keypad_mem;
	printk(KERN_INFO"****************button_init***********\n");
	keypad_mem = request_mem_region(REG_PA_KEYPAD, REG_SZ_KEYPAD, "s3c-keypad");
	if (keypad_mem == NULL) {
		printk(KERN_ERR "failed to get memory region\n");
		error = -ENOENT;
		return error;
	}

    printk(KERN_INFO"1111111111111111111111\n");
	key_base = ioremap(REG_PA_KEYPAD, REG_SZ_KEYPAD);
	if (key_base == NULL) {
		printk(KERN_ERR"Failed to remap register block\n");
		error = -ENOMEM;
		goto err_map;
	}

    printk(KERN_INFO"22222222222222222222222222222222\n");
    writel(readl(key_base + S3C_KEYIFCON) & ~(INT_F_EN|INT_R_EN),key_base + S3C_KEYIFCON);
    writel(KEYIFSTSCLR_CLEAR, key_base + S3C_KEYIFSTSCLR);

    printk(KERN_INFO"33333333333333333333333333333\n");
	//申请中断
	//if (request_irq(BUTTON_IRQ, button_interrupt, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "mihu-button", NULL)) {
//	if (request_irq(BUTTON_IRQ, button_interrupt, 0, "mihu-button", NULL)) {
//		printk(KERN_ERR "button.c: Can't allocate irq \n");
//		goto err_irq;
//    }
    
//	printk(KERN_INFO"****************request_irq***********\n");
	//分配输入设备
	button_dev = input_allocate_device();
	if (!button_dev) {
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq;
	}
	//设置外设时钟
//	keypad_clock = clk_get(&button_dev->dev, "keypad");
//	clk_enable(keypad_clock);
	//配置寄存器
//	writel(KEYIFCON_INIT, key_base+S3C_KEYIFCON);  
//	writel(KEYIFFC_DIV, key_base+S3C_KEYIFFC);
	
#if defined(CONFIG_KEYPAD_S3C_MSM)
	s3c_setup_keypad_cfg_gpio();
#else
	s3c_setup_keypad_cfg_gpio(KEYPAD_ROWS, KEYPAD_COLUMNS);//设置GPIO功能
#endif	

	writel(KEYIFCOL_CLEAR, key_base+S3C_KEYIFCOL);// tri-state buffer enable

	set_bit(EV_KEY, button_dev->evbit);//事件类型是  按键事件
	for(key = 0; key < MAX_KEYPAD_NR; key++){
		code = keypad_keycode[key];
		if (code <= 0)
			continue;
		set_bit(code & KEY_MAX, button_dev->keybit);//设置按键编码
	}

    button_dev->name = "mihu-button";
	button_dev->keycode = keypad_keycode;
	
	error = input_register_device(button_dev);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev;
	
    }

    printk(KERN_INFO"input device register success\n");

    keypad_clock = clk_get(&button_dev->dev, "keypad");
    clk_enable(keypad_clock);
        
             
    writel(KEYIFFC_DIV, key_base+S3C_KEYIFFC);
    writel(KEYIFCON_INIT, key_base+S3C_KEYIFCON); 
    //writel((KEYIFCON_CLEAR | INT_F_EN|DF_EN|FC_EN), key_base+S3C_KEYIFCON);
    
    if (request_irq(BUTTON_IRQ, button_interrupt, 0, "mihu-button", NULL)) {
                printk(KERN_ERR "button.c: Can't allocate irq \n");
                        goto err_irq;
    }
	return 0;

err_irq:

    input_unregister_device(button_dev);
 err_free_dev:
	input_free_device(button_dev);
 err_free_irq:
//	free_irq(BUTTON_IRQ, button_interrupt);
 //err_irq:
	iounmap(key_base);
 err_map:
	release_mem_region(REG_PA_KEYPAD, REG_SZ_KEYPAD);

	return error;
}

static void __exit button_exit(void)
{
    
	free_irq(BUTTON_IRQ, button_interrupt);
    input_unregister_device(button_dev);
	//input_free_device(button_dev);  use input_unregister_device() and memory will be freed once last  reference to the device is dropped.
	//free_irq(BUTTON_IRQ, button_interrupt);
	iounmap(key_base);
	release_mem_region(REG_PA_KEYPAD, REG_SZ_KEYPAD);
}



module_init(button_init);
module_exit(button_exit);



MODULE_AUTHOR("mihu525");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("KeyPad interface for QT210");


