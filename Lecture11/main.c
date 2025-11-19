/*
  player_action_annotated.c

  목적: 함수포인터 개념을 학생용으로 연습하게 하기 위한 예제.
  - 함수포인터 typedef 정의
  - 여러 행동 함수 정의 (aggressive, defensive, random)
  - 구조체 내부에 함수포인터 저장
  - 런타임에 전략(함수)을 교체해서 호출해 보기

  컴파일(리눅스/맥): gcc -o player_action_annotated player_action_annotated.c
  실행: ./player_action_annotated
*/

#include <stdio.h>   // printf
#include <stdlib.h>  // rand, srand
#include <time.h>    // time (난수 시드 설정)

/* -----------------------------------------------------------------------
   1) 함수포인터 타입 정의 (typedef)

   typedef int (*ActionFunc)(int hp);

   - ActionFunc 는 "함수 포인터 타입 이름"이다.
   - 이 타입은 "int를 받아서 int를 반환하는 함수"를 가리킨다.
     구체적으로: 파라미터로 현재 HP(int)를 받고, 그 턴의 데미지(int)를 반환.
   - 왜 typedef? 코드를 읽기 쉽게 하기 위해서다.
     실제 선언을 매번 쓰면 복잡하니 ActionFunc 라는 별칭을 만든다.
   ----------------------------------------------------------------------- */
typedef int (*ActionFunc)(int hp);

/* -----------------------------------------------------------------------
   2) 행동 함수들 (실제 구현)
   각 함수는 ActionFunc 타입의 시그니처(형태)를 가진다: int f(int)

   함수 내부에는 간단한 출력(어떤 행동을 했는지)과 데미지 반환이 있다.
   학생들에게는 "실제 게임에서는 여기에 공격 판정, 쿨다운, 애니메이션 트리거" 등이 들어간다고 설명.
   ----------------------------------------------------------------------- */

   /* 공격형: 항상 공격, 고정 데미지 10 */
int aggressive_action(int hp) {
    // hp 매개변수는 현재 체력이다. (여기서는 단순히 출력용)
    printf("[Aggressive] 공격한다! (현재 HP=%d)\n", hp);
    return 10;  // 이 턴에 상대에게 주는 데미지
}

/* 방어형: 공격하지 않음(데미지 0) — 방어 자세 */
int defensive_action(int hp) {
    printf("[Defensive] 방어 자세 유지. (현재 HP=%d)\n", hp);
    return 0;   // 공격을 하지 않음
}

/* 랜덤형: 50% 확률로 공격(5), 50% 확률로 방어(0) */
int random_action(int hp) {
    int r = rand() % 2; // 0 또는 1
    if (r == 0) {
        printf("[Random] 공격! (현재 HP=%d)\n", hp);
        return 5;
    }
    else {
        printf("[Random] 방어! (현재 HP=%d)\n", hp);
        return 0;
    }
}

/* -----------------------------------------------------------------------
   3) Player 구조체: 함수포인터를 멤버로 갖는다

   typedef struct {
       int hp;
       ActionFunc act;
   } Player;

   - 이 구조체는 '행동 전략'을 필드로 가짐으로써
     같은 Player 인스턴스가 서로 다른 전략을 가질 수 있게 한다.
   - 이 패턴은 C로 전략패턴(Strategy pattern)을 흉내내는 방법이다.
   ----------------------------------------------------------------------- */
typedef struct {
    int hp;
    ActionFunc act;   // 행동을 가리키는 함수포인터
} Player;

/* -----------------------------------------------------------------------
   4) main: 실제 실행 흐름
   - srand로 난수 시드 초기화 (random_action 사용을 위해)
   - Player 생성 및 기본 전략 설정
   - 세 턴 동안 전략을 교체하며 act 호출
   ----------------------------------------------------------------------- */
int main() {
    /* 난수 초기화: random_action에서 사용 */
    srand((unsigned)time(NULL));

    /* Player 변수 선언 및 초기값 설정 */
    Player p;
    p.hp = 100;                // 초기 HP
    p.act = aggressive_action; // 기본 전략: 공격형

    /* ------- 턴 1: 기본 전략 (aggressive) ------- */
    printf("=== 턴 1 ===\n");
    // 함수포인터를 통해 호출: p.act(p.hp)
    // -> 이 호출과 일반 함수 호출의 차이는 없다(단지 호출 대상이 변수라는 점).
    int dmg = p.act(p.hp);
    printf("턴1 데미지 = %d\n\n", dmg);

    /* ------- 전략 변경: Defensive ------- */
    printf("전략을 Defensive 로 교체!\n");
    p.act = defensive_action;  // 런타임에 함수포인터를 다른 함수로 바꾼다.

    printf("=== 턴 2 ===\n");
    dmg = p.act(p.hp);         // 이제 defensive_action이 호출된다.
    printf("턴2 데미지 = %d\n\n", dmg);

    /* ------- 또 다른 전략 변경: Random ------- */
    printf("전략을 Random 으로 교체!\n");
    p.act = random_action;

    printf("=== 턴 3 ===\n");
    dmg = p.act(p.hp);
    printf("턴3 데미지 = %d\n\n", dmg);

    /* 프로그램 끝. */
    return 0;
}