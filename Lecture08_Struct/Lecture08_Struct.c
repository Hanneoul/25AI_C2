// Lecture08_Struct.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

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
	struct Fish fishes[5];
	
	printf("찌를 던진다 (Y/N) >");
	char yn;
	yn = _getch();
	printf("%c\n", yn);

	printf(".....\n"); //(500ms마다 .하나 찍히게)

	printf("%s : " 물고기이름, 질문);	
	
	return 0;

}


//(1반) 과제물
//
// 1. "찌를 던진다 (Y/N) >"  출력
// 키 입력을 받는다. (y,n키) 말고 예외도 처리하도록

// 2. 500ms마다 .을 찍어서 총 5개의 점이 찍힌 후에 "입질이 왔다!!!!" 녹색으로 출력

// 3. 랜덤하게 5개의 물고기중 하나가 튀어나와서 말을건다. talk에 있는 말을. 여기엔 퀴즈를 집어넣고 answer에 답을 넣어둔다.

// 4. 퀴즈를 풀어서 맞추면 talkO에 해당하는 대사를 출력시키고, 틀리면 talkX대사를 출력시킨후 

// 5. 로그 파일을 log.txt라고 만들고 물고기 이름과, 퀴즈 맞췄는지여부를 저장하고 프로그램 종료시킨다.
//    "세자르\n"
//    "X\n"
//    **특징 : 계속 누적돼야한다.
//             log.txt는 지워도, 프로그램을 실행시키면 새로 생겨야한다.
//

// 6. 게임을 켤때마다 "n번째 게임입니다!" 하고 플레이 횟수 및 정답률을 출력한다.
//ex>     7번째 게임입니다!!
//        정답률 4/7
