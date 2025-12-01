/* main_static.c
 *
 * 이 프로그램은 정적 라이브러리(staticlib.lib)와 링크해서 사용한다.
 * "정적 링크 후 exe만 있으면 실행 가능"을 보여줌.
 */

#include <stdio.h>
#include "fun.h"

int main(void) {
    printf("=== Static library demo ===\n");
    printf("Who am I? %s\n", who_am_i()); /* 정적 라이브러리의 문자열 출력 */

    /* 여러 번 roll 호출하여 항상 같은(예측 가능한) 결과를 보여줌 */
    for (int i = 0; i < 5; ++i) {
        int r = roll();
        printf("Roll %d -> %d\n", i + 1, r);
    }

    return 0;
}
