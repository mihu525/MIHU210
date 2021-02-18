/*************************************************************************
	> File Name: cli.c
	> Author: mihu525
	> Mail: mihu525@qq.com 
	> Created Time: 2020年05月01日 星期五 00时57分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


#define SERVER_LANIP  "192.168.3.36"

#define SER_PORT 1224


int main()
{
    int sockfd = -1;
    struct sockaddr_in ser_addr;
    char buf[500] = {0};
    

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(-1);

    }

    memset(&ser_addr, 0, sizeof(struct sockaddr_in));


    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SER_PORT);
    ser_addr.sin_addr.s_addr = inet_addr(SERVER_LANIP);


    if( connect(sockfd, (struct sockaddr *)&ser_addr,sizeof(struct sockaddr_in) )<0)
    {
        perror("connect");
        exit(-1);
    }

    printf("connect sucess\n");

    while(1)
    {
        int count = -1,rel_count = -1;
        gets(buf); 
        
        count = strlen(buf);

        printf("You write [%d] bytes ,[%s]\n", count, buf);

        rel_count = write(sockfd, buf, count);
        if(rel_count < 0)
        {
            perror("write");
            exit(-1);
        }

        if(strcmp(":q",buf)==0)
            return 0;
        memset(buf, 0, count);
        
    }
    
    return 0;
}

