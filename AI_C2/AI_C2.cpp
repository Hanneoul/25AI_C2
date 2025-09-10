// AI_C2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <math.h>



int drawPoint(int x, int y)
{
    printf("\x1B[%d;%dH*",y,x);
    return 0;
}

int main()
{
    int posx, posy;
    posx=3, posy=3;
    double PI = 3.14159265358979323846;

    double angle = 0;
    int length = 0;


    while (angle < 360)
    {
        int y, x;
        
        x = cos(angle * PI / 180.0);
        if(x)
            length = -5;
        else
            length = 0;
        y = sin(angle * PI / 180.0);

        while (length < 5)
        {
            posx += x;
            posy += y;
            drawPoint(posx, posy);
            length++;
        }
        angle += 90;
    }

    printf("\x1B[10;0H");
}
