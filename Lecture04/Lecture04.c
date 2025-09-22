#include <stdio.h>

int main()
{
    char a[6];

    char a[0] = 'L';
    char a[1] = 'E';
    char a[2] = 'S';
    char a[3] = '_';
    char a[4] = 'G';
    char a[5] = 'O';

    int i = 0;
    for (i=0;i<6;i++)
    {
        printf("%c", a[i]);
    }
    return 0;
}