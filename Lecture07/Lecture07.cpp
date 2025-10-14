#include <stdio.h>

int main()
{
    int number[16][8];  //0: ' '(공백), 1: '@'

    int i = 0;
    for (i=0;i<8; i=i+1)
    {
        int j = 0;
        for (j = 0; j < 16; j++)
        {
            number[j][i] = i * 2 +j;
            printf("%d ", number[j][i]);
            
        }
        printf("\n");
    }


    printf("%d", *(number[15]+1));
}
