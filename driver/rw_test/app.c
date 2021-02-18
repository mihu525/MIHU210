/*************************************************************************
	> File Name: app.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月05日 星期二 17时30分45秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEV_FILE "/dev/rw_test"

int main(void)
{
    int fd = -1;

    fd = open(DEV_FILE, O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(-1);
    }

    printf("open success\n");
    sleep(1);

    close(fd);

    return 0;

}
