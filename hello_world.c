#include <stdio.h>



int main()
{
	printf("hello world!\n");
	printf("~/.ssh/\n");
	//first time
	printf("echo >> \"xxx\" >> README.md\n");
	printf("git init\n");
	printf("git add README.md\n");
	printf("git commit -m \"first commit\"\n");
	printf("git remote add origin https://github.com/mihu525/hello-world.git\n");
	printf("git push -u origin master\n");

	//second time 
	printf("git add hello_world.c\n");	
	printf("git remote add origin https://github.com/mihu525/hello-world.git\n");
        printf("git push -u origin master\n");

	return 0;	
}
