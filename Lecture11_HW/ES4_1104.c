int ES4action(int hp, char* name)
{
    int r = rand() % 2; // 0 ¶Ç´Â 1
    if (r == 0) {
        return Wait(hp, name);
    }
    else {
        return Heal(hp, name);

    }
}