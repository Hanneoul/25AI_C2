#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

// 1. 객체의 구조체 정의
typedef struct {
    char name[50];
    int health;
    int attack_power;
} GameObject;

// 2. 외부에서 정의된 객체 변수의 '선언' (extern 사용)
// main.c에서 이 객체를 사용할 것임을 알립니다.
extern GameObject player;

// 3. 외부에서 정의된 액션 함수의 '선언' (프로토타입)
void initialize_player(const char* name, int hp, int ap);
int attack_target(GameObject* attacker, GameObject* target);
void print_status(GameObject* obj);

#endif // GAME_OBJECT_H