/*************************************************************************
	> File Name: appfb.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月24日 星期日 19时12分23秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>




#define FDPATH "/dev/graphics/fb0"

//#define FDPATH "/dev/fb0"

unsigned int *pfb = NULL;
struct fb_var_screeninfo var;   /* Current var */
struct fb_fix_screeninfo fix;   /* Current fix */

#define WIDTH       800    
#define HEIGHT      (480)


#define WHITE       0xffffffff          // test ok
#define BLACK       0x00000000
#define RED         0xffff0000
#define GREEN       0xff00ff00          // test ok
#define BLUE        0xff0000ff          




void draw_back(unsigned int width, unsigned int height, unsigned int color)
{
    unsigned int x, y;
    
    for (y=0; y<height; y++)
    {
        for (x=0; x<width; x++) 
        {            
            *(pfb + y * WIDTH + x) = color;
        }
        
    }
}

void draw_line(unsigned int color)
{
    unsigned int x, y;
    
    for (x=50; x<600; x++)
    {
            
        *(pfb + 200 * WIDTH + x) = color;
        
    }
}



int main()
{
    int fd = -1;

    printf("hello world \n");

    fd = open(FDPATH, O_RDWR);
    if(fd < 0)
    {
        printf("["FDPATH"]");
        printf("fd = %d\n",fd);
        printf("errno = %d\n",errno);
        perror("open");
        return -1;
    }

    if(ioctl(fd, FBIOGET_FSCREENINFO, &fix)<0)
    {
        perror("ioctl fix");
        close(fd);
        return -1;
    }
    printf("smem_start = 0x%x, smem_len = %u.\n", fix.smem_start, fix.smem_len);

    if(ioctl(fd, FBIOGET_VSCREENINFO, &var)<0)
    {
        perror("ioctl var");
        close(fd);
        return -1;
    }
    printf("xres = %u, yres = %u.\n", var.xres, var.yres);
    printf("xres_virtual = %u, yres_virtual = %u.\n", var.xres_virtual, var.yres_virtual);
    printf("bpp = %u.\n", var.bits_per_pixel);

    pfb = mmap(NULL,fix.smem_len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if (NULL == pfb)
    {
        perror("mmap");
        return -1;
    }
    printf("pfb = %p.\n", pfb);
            
    draw_back(WIDTH, HEIGHT, GREEN);
    draw_line(RED);
    munmap(pfb,fix.smem_len);

    close(fd);
}


