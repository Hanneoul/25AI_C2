#include <stdio.h>

/*
    이 파일(a.c)에서만 보이게 숨기고 싶은 전역 변수.
    -> static 전역변수 = 내부 연결(internal linkage)
    -> 다른 .c 파일에서는 절대 접근 불가능.
*/
static int hidden_value = 10;

/*
    프로그램 전체에서 공유 가능한 전역 변수.
    -> 일반 전역변수 = 외부 연결(external linkage)
    -> 다른 .c 파일에서 extern int public_value; 로 선언하면 접근 가능.
*/
int public_value = 20;

/*
    이 함수는 hidden_value와 public_value를 출력한다.
    main.c에서 사용 테스트를 위해 공개한다.
*/
void PrintValues(void)
{
    printf("[a.c] hidden_value = %d\n", hidden_value);
    printf("[a.c] public_value = %d\n", public_value);
}
