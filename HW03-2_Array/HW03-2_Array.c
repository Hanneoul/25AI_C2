#include <stdio.h>
#include <stdlib.h>

#define Key_a 97
#define Key_z 122

int swap(char* a, char* b)
{
    char c;
    c = *a;
    *a = *b;
    *b = c;
}

int main()
{
    char random[Key_z - Key_a + 1];

    //배열 초기화
    int i = 0;
    for (i = 0; i < Key_z - Key_a + 1; i++)
    {
        random[i] = 0;
    }


    
    for (i = 0; i < Key_z - Key_a + 1; i++)
    {
        while (1)
        {
            int rand_index = rand() % (Key_z - Key_a + 1);
            if (random[rand_index] == 0)
            {
                random[rand_index] = (char)(i+ Key_a);
                break;
            }
        }
    }
    //소팅해보기

    random[0] = 'a';
    random[1] = 'b';

    swap(&random[0], &random[1]);




    //------------순서대로 숫자 정렬------------------------
    printf("\t\trandom\tsort");
    for (i = 0; i < Key_z - Key_a + 1; i++)
    {
        printf("\n\t%d\t%c\t%c", i, random[i]);
    }


}


