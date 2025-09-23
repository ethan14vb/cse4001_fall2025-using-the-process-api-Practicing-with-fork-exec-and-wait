#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
	int x;
	x = 100;

	int rc = fork();

	if (rc < 0) 
	{
		fprintf(stderr, "Fork Failed \n");
		exit(1);
	}
	else if (rc == 0) 
	{
		x = 5;
		printf("I am child %d\n", x);
	}
	else
	{
		x = 15;
		printf("I am parent %d\n", x);
	}
	return 0;
}
