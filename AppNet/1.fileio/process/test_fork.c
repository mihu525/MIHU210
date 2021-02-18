#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

void atexit_cb(void)
{
	printf("atexit_cb:PID = %d\n",getpid());
}
int main(void)
{
	pid_t pid;
	printf("hello world!\n");
//	atexit(atexit_cb);
	pid = fork();
	
	atexit(atexit_cb);
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}

	if(pid > 0)
	{
		printf("please call me father\n");
		printf("PID = %d\n",getpid());
		sleep(1);
	}
	if(pid == 0)
	{
		printf("I'm son\n");
		printf("PID = %d, PPID = %d\n",getpid(),getppid());
	}
	return 0;
}
