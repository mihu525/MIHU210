#include <stdio.h>


int main()
{
	FILE *fp = NULL;
	fp = fopen("lib.c","a+");
	if(fp == NULL)
	{
		printf("open error !");
		return -1;
	}

	while(1)
	{
		int count;
		count = fwrite("cc", 2,1,fp);
		printf("write cc count = %d\n",count);
		sleep(1);
		count = fwrite( "dd",1, 2,fp);

		printf("write dd count = %d\n",count);
		fflush(fp);
	}

	printf("jump the while");
	fclose(fp);
	return 0;
	

}

