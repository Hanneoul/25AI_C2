#include "ES_1101.h"

/* 공격형: 항상 공격, 고정 데미지 10 */
int aggressive_action(int hp) {
    // hp 매개변수는 현재 체력이다. (여기서는 단순히 출력용)
    printf("[Aggressive] 공격한다! (현재 HP=%d)\n", hp);
    return 10;  // 이 턴에 상대에게 주는 데미지
}