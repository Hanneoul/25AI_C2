#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define PI 3.14159226535897

void moveCursor(int x, int y)
{
    printf("\x1b[%d;%dH",y,x);
}

//매개변수 isExploded
//0: 폭발전 폭탄
//1: 폭발함
void printBomb(int isExploded)
{
    //폭탄은 항상 7칸
    if(isExploded)
    { 
        printf("\x1b[A^^^^^^^");
        printf("\x1b[B\x1b[7D!!BAM!!"); 
        printf("\x1b[B\x1b[7D^^^^^^^");
    }
    else
        printf("(  b  )"); 
}

int main()
{
    int StartPointX = 10;
    int StartPointY = 5;
    int MoveDirX = -1;
    int MoveDirY = 0;
    double angle = PI;
    
    //HINT: 도화선부분부터 그리기
    StartPointX += 4;
    StartPointY += 4;
    moveCursor(StartPointX, StartPointY);   //불 붙는 도화선 부분
    printf("*");

    int i = 0;
    int iCount = 1;
    int j = 0;
    int k = 0;
    for (k = 0; k < 6; k++)
    {
        for(j=0;j < 2;j++)
        {
            angle -= PI / 2.0;
            MoveDirX = round(cos(angle));
            MoveDirY = round(sin(angle));
            for (i = 0; i < iCount*2; i++)
            {
                StartPointX += MoveDirX;
                StartPointY += MoveDirY;
                moveCursor(StartPointX, StartPointY);   //불 붙는 도화선 부분
                printf("#");
            }                        
        }
        iCount++;
    }

    //폭탄그리기
    moveCursor(StartPointX - 7, StartPointY);   //폭탄 칸만큼 후퇴
    printBomb(0);

    //StartPointX = 14;
    //StartPointY = 9;

    //moveCursor(StartPointX, StartPointY);   //불 붙는 도화선 부분
    //printf("*");
    //
    //Sleep(200);
    //moveCursor(StartPointX, StartPointY);   //불 붙는 도화선 부분
    //printf(" ");

  
    //iCount = 1;
    //
    //for (k = 0; k < 6; k++)
    //{
    //    for (j = 0; j < 2; j++)
    //    {
    //        angle -= PI / 2.0;
    //        MoveDirX = round(cos(angle));
    //        MoveDirY = round(sin(angle));
    //        for (i = 0; i < iCount * 2; i++)
    //        {
    //            StartPointX += MoveDirX;
    //            StartPointY += MoveDirY;
    //            moveCursor(StartPointX, StartPointY);   //불 붙는 도화선 부분
    //            printf("*");
    //            Sleep(200);
    //            moveCursor(StartPointX, StartPointY);   //불 붙는 도화선 부분
    //            printf(" ");
    //        }
    //    }
    //    iCount++;
    //}

    ////폭탄그리기
    //moveCursor(StartPointX - 7, StartPointY);   //폭탄 칸만큼 후퇴
    //printBomb(1);



    moveCursor(10, 20);
    return 0;
}
