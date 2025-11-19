int Heal(int hp) {
    printf("[Heal] 힐! (현재 HP=%d)\n", hp);
    return -10;
}

int Wait(int hp) {
    int r = rand() % 2; // 0 또는 1
    if (r == 0) {
        printf("[Counter] 방어성공! (현재 HP=%d)\n", hp);
        return 10;
    }
    else {
        printf("[Counter] 처맞음! (현재 HP=%d)\n", hp - 20);
        return -20;
    }
}