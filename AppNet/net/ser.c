/*************************************************************************
	> File Name: ser.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年04月30日 星期四 23时14分27秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_WANIP "111.199.53.165"
#define SERVER_LANIP  "192.168.3.36"
#define LISTEN_BACKLOG 50
#define SER_PORT 1224

int main()
{
    int listen_fd = -1;
    int cli_fd = -1;
    int ret = -1;
    struct sockaddr_in ser_addr,cli_addr;
    socklen_t len;
    char buf[500] = {0};

   //. pritf("buf size is %d \n",SSIZE_MAX);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        perror("socket");
        exit(-1);
    }

    memset(&ser_addr, 0, sizeof(struct sockaddr_in));

    
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SER_PORT);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_LANIP);
    ret = bind(listen_fd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        perror("bind");
        exit(-1);
    }

    ret = listen(listen_fd, LISTEN_BACKLOG);
    if(ret < 0)
    {
        perror("listen");
        exit(-1);
    }


    memset(&cli_addr, 0, sizeof(struct sockaddr_in));
    //len = 0;  output 0.0.0.0
    len = sizeof(struct sockaddr_in);
    cli_fd = accept(listen_fd, (struct sockaddr *)&cli_addr,&len);

    if(cli_fd<0)
    {
        perror("accept");
        exit(-1);
    }
    
    printf("fd = %d, cli ip is %s\n",cli_fd,inet_ntoa(cli_addr.sin_addr));

    while(1)
    {
        ret = -1;
        ret = read(cli_fd, buf, 500);
        if(ret < 0)
        {
            perror("read");
            exit(-1);
        }

        printf("[%s][%d]:[%s]\n",inet_ntoa(cli_addr.sin_addr),strlen(buf),buf);
        if(strcmp(":q",buf)==0)
        {
            return(0);
        }
        memset(buf,0,strlen(buf));


    }
    return 0;
}


