
#include <stdio.h>
#include <string.h>
#include "game_object.h"

// 1. extern으로 선언된 'player' 객체의 실제 '정의' (초기화는 나중에 함수로)
GameObject player;

// 2. 플레이어 초기화 함수 구현
void initialize_player(const char* name, int hp, int ap) {
    strncpy_s(player.name, 49, name, 49);
    player.name[49] = '\0';
    player.health = hp;
    player.attack_power = ap;
}

// 3. 공격 액션 함수 구현
int attack_target(GameObject* attacker, GameObject* target) {
    if (target->health <= 0) {
        printf("%s는 이미 파괴되었습니다.\n", target->name);
        return;
    }

    int damage = attacker->attack_power;
    target->health -= damage;

    printf("%s가 %s에게 공격! %d의 피해를 입혔습니다.\n",
        attacker->name, target->name, damage);

    if (target->health <= 0) {
        printf(">>> %s가 파괴되었습니다! <<<\n", target->name);
        target->health = 0; // 체력을 0으로 설정하여 음수가 되는 것을 방지
        return 0;
    }
    return 1;
}

// 4. 상태 출력 함수 구현
void print_status(GameObject* obj) {
    printf("[상태] 이름: %s, 체력: %d, 공격력: %d\n",
        obj->name, obj->health, obj->attack_power);
}