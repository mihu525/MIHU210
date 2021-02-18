/*************************************************************************
	> File Name: input.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月28日 星期四 21时48分18秒
 ************************************************************************/

#include <stdio.h>
#include <linux/input.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//#define DEV_KEY     "/dev/input/event1"
//#define DEV_MOUSE   "/dev/input/event4"


#define DEV_KEY     "/dev/input/event2"
#define DEV_TS   "/dev/input/event0"




void * fun_key(void *parg)
{
    printf("-----fun_key---------\n");
    struct input_event ev;
    int fd = -1;
    fd =  open(DEV_KEY,O_RDONLY);
    if(fd < 0)
    {
        printf("open key failed \n");
        return NULL;
    }
    while(1)
    {
        ssize_t ret = 0;
        ret = read(fd,&ev,sizeof(ev));
        if(ret != sizeof(ev))
        {
            printf("key read failed\n");
            break;
        }

        printf("[key]:type = %d code = %d value = %d \n",ev.type,ev.code,ev.value);
    }

    close(fd);

    return NULL;
}
void *fun_mouse(void *parg)
{
    printf("-----fun_mouse---------\n");
    struct input_event ev;
    int fd = -1;
    fd =  open(DEV_TS,O_RDONLY);
    if(fd < 0)
    {
        printf("open ts failed \n");
        return NULL;
    }
    while(1)
    {
        ssize_t ret = 0;
        ret = read(fd,&ev,sizeof(ev));
        if(ret != sizeof(ev))
        {
            printf("ts read failed\n");
            break;
        }

        printf("[ts]:type = %d code = %d value = %d \n",ev.type,ev.code,ev.value);
    }

    close(fd);
    return NULL;
}


int main()
{
    pthread_t th_key = 0, th_mouse = 0;
    int ret = -1;
    ret = pthread_create(&th_key,NULL,fun_key,NULL);
    if(ret)
    {
        printf("key pthread_create failed\n");
        printf("error number is %d",ret);
        return -1;
    }


    ret = pthread_create(&th_mouse,NULL,fun_mouse,NULL);
    if(ret)
    {
        printf("mouse pthread_create failed\n");
        printf("error number is %d",ret);
        return -1;
    }


    while(1)
    {
        char q;
        scanf("%c",&q);
        if(q == 'q')
        {
            printf("bye-bye\n");
            return 0;
        }
    }

}

