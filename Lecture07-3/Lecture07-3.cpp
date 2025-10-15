#include <stdio.h>

int aa;

int f()
{
	int aaa;
	return 0;
}

int main()
{
	int a = sizeof(int);
	f();
	int chess[10][10];

	int i = 0;
	int j = 0;
	
	for (j = 0; j < 10; ++j)
	{
		for (i = 0; i < 10; i++)
		{
			chess[i][j] = i + j;
			printf("\t%d", chess[i][j]);
			printf("\t%d",*(*(chess+i)+j));
		}
		printf("\n");
	}
}
	