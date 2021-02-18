/*************************************************************************
	> File Name: app.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月23日 星期六 21时15分34秒
 ************************************************************************/

#include<stdio.h>
#include <fcntl.h>
#define file "/dev/led4"

#define IOCTRL_OPEN  1
#define IOCTRL_CLOSE  0

int main()
{
    int fd = -1;

    fd = open(file,O_RDWR);

    if(fd<0)
    {
        perror("open");
        return -1;
    }


    ioctl(fd, IOCTRL_OPEN);
    sleep(3);
    ioctl(fd, IOCTRL_CLOSE);
    sleep(3);
    ioctl(fd, IOCTRL_OPEN);
    sleep(3);
    close(fd);

    return 0;
}
