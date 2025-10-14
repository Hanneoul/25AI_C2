#include <stdio.h>

int main()
{
    char a[6];

    a[0] = 'L';
    a[1] = 'E';
    a[2] = 'S';
    a[3] = '_';
    a[4] = 'G';
    a[5] = 'O';

    int i = 0;
    for (i=0;i<6;i++)
    {
        printf("%c", a[i]);
    }
    return 0;
}