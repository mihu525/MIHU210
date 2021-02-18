/*************************************************************************
	> File Name: app.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月05日 星期二 17时30分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_FILE "/dev/led_test"

char buf[100] = {0};
int main(void)
{
    int fd = -1;
    char led = 0;
    int i = 10;
    fd = open(DEV_FILE, O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(-1);
    }

    printf("open success\n");

    while(i--)
    {
        led = 0xF;
        write(fd,&led,1);
        sleep(2);
        led = 0x0;
        write(fd,&led,1);
        sleep(2);

    }
    close(fd);

    return 0;

}
