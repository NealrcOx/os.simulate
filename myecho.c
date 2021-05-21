/*myecho.c--this programmer bu be like echo*/
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("Error:enter parameter!\n");
		exit(0);
	}
	for(int i = 1 ; i < argc ; i++)
		printf("%s ", argv[i]);
	printf("\n");
	return 0;
}
