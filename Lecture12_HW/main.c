#include <stdio.h>
#include "game_object.h"

int main() {
    // game_object.c에 정의된 'player' 객체와 함수들을 사용합니다.

    // 1. player 객체 초기화 (game_object.c의 함수 호출)
    initialize_player("용감한 기사", 100, 20);

    // 2. 임시 적 객체 생성 (main.c 내부 정의)
    GameObject goblin = { "고블린", 30, 5 };

    printf("--- 게임 시작 ---\n");
    print_status(&player); // player 상태 출력 (extern 사용)
    print_status(&goblin);
        

    int result = 1;
    int count = 0;
    // 3. player가 goblin을 공격 (game_object.c의 함수 호출)
    while (result)
    {
        printf("\n--- 액션 : 공격 %d---\n", count);
        if(count%2)
            result = attack_target(&player, &goblin);
        else
            result = attack_target(&goblin, &player);
        print_status(&player);
        print_status(&goblin);
        count++;
    }
    
    return 0;
}