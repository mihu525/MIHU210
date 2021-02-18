#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
	int fd = -1;
	fd = open("api.c",O_CREAT|O_RDWR);
	if(fd == -1)
	{
		printf("open error !");
		return -1;
	}

	while(1)
	{
		write(fd, "aa", 2);
		sleep(1);
		write(fd, "bb", 2);
	}

	printf("jump the while");
	close(fd);
	return 0;
	

}

