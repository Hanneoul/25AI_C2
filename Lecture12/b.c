#include <stdio.h>

/*
    여기서 public_value는 '외부에 정의된 것'이라고 알려주는 선언.
    -> extern 은 "이거 다른 파일에 진짜 정의가 있다"는 약속일 뿐.
    -> 메모리도 만들지 않음.
*/
extern int public_value;

/*
    hidden_value에 접근한다고 선언하려 시도하면?
    -> 컴파일은 되지만 링크 단계에서 터짐.
    -> 왜냐면 hidden_value는 a.c 내부 static → 외부에 노출 X
*/
// extern int hidden_value; // 주석 풀면 링크 에러 발생함.

void ModifyValues(void)
{
    public_value = 999;  // 가능
    // hidden_value = 777; // 불가능 (링크 에러)

    printf("[b.c] public_value modified to %d\n", public_value);
}
