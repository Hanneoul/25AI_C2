#include <stdio.h>
#include <stdlib.h>

#define NUM 10000

int main()
{
    //랜덤한 숫자 구하기
    int r_value = rand() % NUM;

    int random[NUM];
    int index[NUM];

    //-----------초기화-------------------------
    int i;
    for (i = 0; i < NUM; i++)
    {
        random[i] = -1;
        index[i] = -1;
    }

    //------------랜덤하게 숫자 넣기------------------------
    for (i = 0; i < NUM; i++)
    {
        while (1)
        {
            int rand_index = rand() % NUM;
            if (random[rand_index] == -1)
            {
                random[rand_index] = i;
                break;
            }
        }
    }

    //------------순서대로 숫자 정렬------------------------
    for (i = 0; i < NUM; i++)
    {
        index[random[i]] = i;
    }

    printf("\t\trand\tind");
    for (i = 0; i < NUM; i++)
    {
        printf("\n\t%d\t%d\t%d", i, random[i], index[i]);
    }










    return 0;
}