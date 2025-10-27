#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>   // 오늘의 주인공
#include <stdlib.h>


int main(void)
{
    char input[100];
    fgets(input, sizeof(input), stdin);
    
    
    // 간단한 하이라이트: 'o'를 '*'로 모두 바꾸기(파괴적)
    char buf[128];
    strcpy(buf, input);
    for (char* q = strstr(buf, "gay"); q != NULL; q = strstr(q + 1, "gay")) 
    {
        strncpy(q, "***",3);
    }
    printf("replace 'gay'->'***': \"%s\"\n", buf);


    return 0;
}
