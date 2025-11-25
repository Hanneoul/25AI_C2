/*
===============================================================
    [강의 노트: extern 과 static, 이 둘을 왜 같이 가르쳐야 하는가]

    1) C의 전역 변수는 “링키지(linkage)” 개념이 붙는다.
       - 외부 링크(external linkage)
       - 내부 링크(internal linkage)

    2) extern  (외부 링크 / External Linkage)
       - 의미: "이거 다른 파일에 실제 정의가 있으니 여기서 참조만 할게."
       - 메모리 생성 안 함. 오직 '선언'만 한다.
       - 전역을 여러 .c 파일에서 공유하고 싶을 때 사용.
       - 한 파일에서 정의하고(메모리 생성), 나머지는 extern으로 선언.

       요약: extern = “주소록에만 이름 적는다. 집은 다른 동네에 있다.”

    3) static (파일 범위에서 사용될 때) – 내부 링크 / Internal Linkage
       - 의미: "이 전역은 이 파일 내부 전용이다. 밖에서 절대 보이지 마."
       - 다른 .c 파일에서 extern으로 접근하려고 해도 링커 단계에서 무조건 실패.
       - 정보 은닉(캡슐화) 목적에 좋음.

       요약: static = “문 잠그고 ‘출입금지’ 붙여둔 전역.”

    4) 비교
          ┌───────────────────────┬──────────────────────────┐
          │      extern          │          static           │
          ├───────────────────────┼──────────────────────────┤
          │ 다른 파일에서 보임    │ 다른 파일에서 절대 안 보임 │
          │ 선언(약속) 용도        │ 은닉(숨김) 용도           │
          │ 메모리 생성 X          │ 정의한 파일에서만 접근     │
          └───────────────────────┴──────────────────────────┘

    5) 왜 헷갈리나?
       - 전역이 하나만 있으면 extern도 static도 필요 없다고 착각함.
       - 실제로 프로젝트가 .c 파일 여러 개로 나뉘면 linkage 개념이 바로 드러난다.
       - 실습에서 static 전역을 extern으로 불러오면 “링커 에러”가 나며 확실히 이해됨.

    6) 결론
       - extern = "밖에서도 써라"
       - static = "이 파일에서만 써라"
       - 이 둘을 비교해서 가르치면 바로 감 잡는다.

===============================================================
*/

#include <stdio.h>

/*
    a.c에 있는 함수. 외부에 공개되므로 extern 없이도 선언 가능.
*/
void PrintValues(void);

/*
    public_value에 접근하기 위한 extern 선언.
    -> 실제 정의는 a.c에 있음.
*/
extern int public_value;

/*
    b.c에 있는 함수. 값 변경 테스트.
*/
void ModifyValues(void);

int main(void)
{
    printf("=== Before Modification ===\n");
    PrintValues();  // hidden_value=10, public_value=20

    printf("\n=== Modify from b.c ===\n");
    ModifyValues(); // public_value=999로 변경

    printf("\n=== After Modification ===\n");
    PrintValues();  // hidden_value는 그대로 10, public_value는 999

    return 0;
}
