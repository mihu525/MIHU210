/*************************************************************************
	> File Name: test_execl.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年04月22日 星期三 23时14分40秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

extern char **environ;

//int main(void)
int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        printf("[%s]\n",argv[1]);
        execl(argv[1],argv[1],NULL);
    }
    
    printf("test_execl is begin!!\n");
    execl("/bin/ls","ls","-l",NULL);
    printf("test_execl is over!!\n");

    
    return 0;
}

