#include <stdio.h>

int Heal(int hp, char* name) {
    printf("[%s] 힐! (현재 HP=%d)\n", name, hp);
    return -10;
}

int Wait(int hp, char *name) {
    int r = rand() % 2; // 0 또는 1
    if (r == 0) {
        printf("[%s] 방어성공! (현재 HP=%d)\n", name, hp);
        return 10;
    }
    else {
        printf("[%s] 처맞음! (현재 HP=%d)\n", name, hp - 20);
        return -20;
    }
}
