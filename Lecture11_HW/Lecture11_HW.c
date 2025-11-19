#include <stdio.h>
#include "action.h"
//#include "이름_학번.h"

typedef int (*ActionFunc)(int hp);

typedef struct {
    int isDead;
    char* name;
    int hp;
    ActionFunc act;   // 행동을 가리키는 함수포인터
} Player;




int main()
{
    Player p[4];
    
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        p[i].isDead = 0;
        p[i].hp = 100;        
    }

    //함수 각각 플레이어에 연결하는 부분

    while(1)
    {
        for (i = 0; i < 4; i++)
        {
            if (p[i].hp > 0 && !p[i].isDead)
            {
                int point = p[i].act(p[i].hp, p[i].name);
                p[i].hp += point;
            }

            if (!p[i].isDead && p[i].hp <= 0)
            {
                printf("쥬금 : %s", p[i].name);
                p[i].isDead = 1;
            }

        }

        int count = 0;

        for (i = 0; i < 4; i++)
        {
            if (p[i].isDead)
                count++;
        }

        //4명 다 죽으면
        if (count == 4)
            break;
        
    }

    return 0; 
}