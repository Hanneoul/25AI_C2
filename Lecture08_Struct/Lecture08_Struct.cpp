// Lecture08_Struct.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <conio.h>

struct Fish
{
	char* name;
	
	char* talk;		//물고기가 퀴즈를 냄(문제 + 보기(객관식))
	int answer;		//답

	char* talk_O;	//맞았을때 대사
	char* talk_X;	//틀렸을때 대사		
};


//물고기 5개 만들기 Fish 배열로.
int main()
{
	printf("찌를 던진다 (Y/N) >");
	char yn;
	yn = _getch();
	printf("%c\n", yn); //(500ms마다 .하나 찍히게)

	if (yn == 'Y' || yn == 'y')
	{

	}
	else if (yn == 'N' || yn == 'n')
	{

	}
	else
	{
		//반복;
	}

	//printf(".....\n"); //(500ms마다 .하나 찍히게)

	//printf("%s : " 물고기이름, 질문);	
	
	return 0;

}
