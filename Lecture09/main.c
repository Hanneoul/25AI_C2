#define _CRT_SECURE_NO_WARNINGS

/*
    파일명: string_lab.c
    목적  : C의 <string.h>를 실전으로 익히기 위한 "주석 과다" 예제.
            길지만, 이거 한 파일로 수업/실습/퀴즈 전부 커버 가능.

    컴파일:
      - MSVC(Windows): cl /W4 /Za string_lab.c
      - GCC/Clang    : gcc -Wall -Wextra -std=c11 string_lab.c -o string_lab

    이 파일에서 다루는 핵심 함수들(<string.h>):
      길이/복사/붙이기: strlen, strcpy, strncpy, strcat, strncat
      비교           : strcmp, strncmp
      검색           : strchr, strrchr, strstr
      토큰화         : strtok, (MSVC라면) strtok_s
      메모리 블록    : memcpy, memmove, memcmp, memset (보너스)

    ⚠️ 현실적인 주의점(수업 중 반복해서 박아 넣으세요):
      1) "문자열"은 마지막에 '\0'이 있는 char 배열이다. '\0' 없으면 끝 모름 → 폭발.
      2) 버퍼 크기 모르면 strcpy/strcat 같은 "무제한" 함수는 위험. 데모로는 보여주되, 실제 코드는 *_s 또는 길이 제한 버전 사용 습관.
      3) fgets로 입력 받으면 끝에 '\n'이 붙는다 → 직접 제거 필요.
      4) strtok은 파괴적(원본 문자열을 덜어낸다). 원본 보존하려면 복사본에서 토큰화.
      5) memcpy vs memmove: 겹치는 영역(overlap)일 때는 memmove만 안전.

    ─────────────────────────────────────────────────────────────────────────
*/

#include <stdio.h>
#include <string.h>   // 오늘의 주인공
#include <stdlib.h>

//---------------------------------------------
// 유틸 1: 문자열 끝의 개행('\n') 제거기
//---------------------------------------------
void trim_newline(char* s)
{
    if (!s) return;
    size_t len = strlen(s);
    if (len && s[len - 1] == '\n') s[len - 1] = '\0';
}

//---------------------------------------------
// 유틸 2: 안전 입력 (fgets + 개행 제거)
//   - scanf("%s")는 공백에서 끊기고, 버퍼 오버플로우 위험.
//   - fgets는 공백 포함해 읽되, 끝에 '\n'이 남음 → trim 필요.
//---------------------------------------------
void input_line(const char* prompt, char* buf, size_t bufsize)
{
    if (prompt) fputs(prompt, stdout);
    if (fgets(buf, (int)bufsize, stdin) == NULL) {
        // 입력 실패 시, 빈 문자열로
        if (bufsize) buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

//---------------------------------------------
// 데모 1: 길이 함수 (strlen)
//   - sizeof와 strlen 차이 반드시 체감시키기
//---------------------------------------------
void demo_strlen(void)
{
    puts("\n[데모1] strlen vs sizeof");
    char s1[10] = "abc";         // 'a','b','c','\0', 나머지 쓰레기
    char s2[] = "hello";       // 컴파일러가 크기 6으로 잡음(마지막 '\0' 포함)

    printf("s1=\"%s\"  sizeof(s1)=%zu, strlen(s1)=%zu\n",
        s1, sizeof(s1), strlen(s1));
    printf("s2=\"%s\"  sizeof(s2)=%zu, strlen(s2)=%zu\n",
        s2, sizeof(s2), strlen(s2));

    // 실수 데모: 널문자 지우면 strlen이 미쳐 날뛴다(보여주기만 하고 다시 복구)
    char demo[] = "XYZ";
    printf("before: \"%s\" (strlen=%zu)\n", demo, strlen(demo));
    demo[3] = 'Q'; // '\0'을 날려버림 → 이제 끝이 없다(UB 위험)
    // printf("after : \"%s\"\n", demo); // 위험 데모. 출력하지 않는 게 안전.
    demo[3] = '\0'; // 복구
}

//---------------------------------------------
// 데모 2: 복사/붙이기 (strcpy/strncpy, strcat/strncat)
//   - *_n 버전의 'n'은 "소스에서 읽을 최대 길이"지, 목적지 크기를 보장하지 않는다.
//   - 따라서 목적지 총 용량을 알고 있어야 오버플로우를 막을 수 있다.
//   - MSVC/Annex K 사용 가능하면 strcpy_s/strcat_s도 언급.
//---------------------------------------------
void demo_copy_concat(void)
{
    puts("\n[데모2] 복사/붙이기: strcpy/strncpy, strcat/strncat");

    char dest[16];       // 총 용량 16
    char a[] = "hello";
    char b[] = " world!";

    // 1) strcpy: '\0' 포함해서 복사. dest 충분히 커야 함.
    strcpy(dest, a);     // dest := "hello"
    printf("after strcpy: '%s'\n", dest);

    // 2) strcat: dest 끝에 붙임. dest 현재 길이 + 붙일 길이 + 1('\0') <= 용량
    strcat(dest, b);     // dest := "hello world!"
    printf("after strcat: '%s'\n", dest);

    // 3) strncpy: 소스에서 최대 n글자 "읽어" 복사하되, '\0' 보장 X
    //    → 직접 '\0' 보장하거나, 나머지 0 채우기 필요.
    char dest2[8];
    // 'hello'를 7글자까지 읽어 복사. 'hello'는 5글자라 '\0'까지 들어감.
    strncpy(dest2, a, sizeof(dest2) - 1);
    dest2[sizeof(dest2) - 1] = '\0'; // 항상 수동으로 '\0' 보장
    printf("after strncpy (safe): '%s'\n", dest2);

    // 4) strncat: 최대 n글자를 "붙임". n은 붙일 소스 최대 길이.
    char dest3[10] = "he";
    strncat(dest3, "lloXX", 3); // 'llo'만 붙음 → "hello"
    printf("after strncat: '%s'\n", dest3);

    // ✅ Annex K / MSVC 버전(가능한 환경에서 추천)
    //    목적지 크기를 인자로 받아서 오버플로우 방지에 유리.
    // #ifdef _MSC_VER
    //     char safe[16] = {0};
    //     strcpy_s(safe, sizeof(safe), "ABC");
    //     strcat_s(safe, sizeof(safe), "DEF");
    //     printf("MSVC safe funcs: '%s'\n", safe);
    // #endif
}

//---------------------------------------------
// 데모 3: 비교 (strcmp/strncmp)
//   - 사전순 비교: a<b면 음수, 같으면 0, a>b면 양수
//---------------------------------------------
void demo_compare(void)
{
    puts("\n[데모3] 비교: strcmp/strncmp");

    const char* x = "apple";
    const char* y = "apricot";
    const char* z = "apple";

    printf("strcmp('%s','%s') = %d\n", x, y, strcmp(x, y)); // 'l' vs 'r' → 음수
    printf("strcmp('%s','%s') = %d\n", x, z, strcmp(x, z)); // 동일 → 0

    // 접두어 비교: 앞의 3글자만
    printf("strncmp('%s','%s',3) = %d\n", x, y, strncmp(x, y, 3)); // 'apr' vs 'app' → 양수/음수 확인
}

//---------------------------------------------
// 데모 4: 검색 (strchr/strrchr/strstr)
//   - strchr   : 문자 첫 위치
//   - strrchr  : 문자 마지막 위치
//   - strstr   : 부분 문자열 첫 위치
//---------------------------------------------
void demo_search(void)
{
    puts("\n[데모4] 검색: strchr/strrchr/strstr");

    const char* text = "the quick brown fox jumps over the lazy dog";

    const char* p1 = strchr(text, 'o');   // 첫 'o'
    const char* p2 = strrchr(text, 'o');  // 마지막 'o'
    const char* p3 = strstr(text, "own"); // "own" 시작 위치

    printf("text          : \"%s\"\n", text);
    printf("strchr 'o'    : %s\n", p1 ? p1 : "(NULL)");
    printf("strrchr 'o'   : %s\n", p2 ? p2 : "(NULL)");
    printf("strstr \"own\" : %s\n", p3 ? p3 : "(NULL)");

    // 간단한 하이라이트: 'o'를 '*'로 모두 바꾸기(파괴적)
    char buf[128];
    strcpy(buf, text);
    for (char* q = strchr(buf, 'o'); q != NULL; q = strchr(q + 1, 'o')) {
        *q = '*';
    }
    printf("replace 'o'->'*': \"%s\"\n", buf);
}

//---------------------------------------------
// 데모 5: 토큰화 (strtok / strtok_s)
//   - 공백/쉼표/세미콜론 등 구분자로 문자열을 잘라서 "토큰" 나누기
//   - strtok은 원본을 파괴한다. 원본 보존하려면 복사본에서 작업.
//   - 다중 스레드/동시 파싱에는 부적합 → (MSVC) strtok_s 사용 예시 포함.
//---------------------------------------------
void demo_tokenize(void)
{
    puts("\n[데모5] 토큰화: strtok / strtok_s");

    char src[] = "Alice,Bob;Charlie David"; // 원본 파괴될 예정(예제용)
    char copy[64];
    strcpy(copy, src);

    // 1) strtok (표준) — 스레드 안전 X, 내부 static 상태
    //    첫 호출: strtok(문자열, 구분자들)
    //    이어 호출: strtok(NULL, 동일한 구분자들)
    {
        char temp[64];
        strcpy(temp, copy); // 매 호출마다 원본 유지 위해 사본 사용

        const char* delims = " ,;"; // 스페이스/쉼표/세미콜론 모두 구분자
        char* tok = strtok(temp, delims);
        int idx = 0;
        printf("[strtok] tokens:");
        while (tok) {
            printf(" [%d:%s]", idx++, tok);
            tok = strtok(NULL, delims);
        }
        puts("");
    }

    // 2) strtok_s (MSVC 확장, C11 Annex K 유사) — 스레드 안전
    //    인자에 "context"를 둬서 재진입 가능.
#ifdef _MSC_VER
    {
        char temp[64];
        strcpy(temp, copy);

        const char* delims = " ,;";
        char* context = NULL;
        char* tok = strtok_s(temp, delims, &context);
        int idx = 0;
        printf("[strtok_s] tokens:");
        while (tok) {
            printf(" [%d:%s]", idx++, tok);
            tok = strtok_s(NULL, delims, &context);
        }
        puts("");
    }
#endif
}

//---------------------------------------------
// 보너스: 메모리 블록 함수 (memcpy/memmove/memcmp/memset)
//   - 문자열도 결국 바이트 배열 → 메모리 관점 연습용
//---------------------------------------------
void demo_memory_block(void)
{
    puts("\n[보너스] 메모리 블록: memcpy/memmove/memcmp/memset");

    char buf[32] = "ABCDEFGHIJ"; // 10글자 + '\0'
    printf("origin: %s\n", buf);

    // 1) memcpy: 겹치지 않는 구간 복사
    memcpy(buf + 10, buf, 5); // "ABCDEFGHIJABCDE"
    buf[15] = '\0';
    printf("memcpy : %s\n", buf);

    // 2) memmove: 겹치는 구간도 안전
    //    예: [0..9] → [2..11]로 겹치게 이동
    char data[32] = "0123456789";
    memmove(data + 2, data, 10); // 겹침. memmove만 안전.
    data[12] = '\0';
    printf("memmove: %s\n", data);

    // 3) memcmp: 바이트 수준 비교(문자열 규칙X, 길이 반드시 제공)
    printf("memcmp(\"ABC\",\"ABD\",3)=%d\n", memcmp("ABC", "ABD", 3));

    // 4) memset: 특정 값으로 메모리 채우기
    char z[8];
    memset(z, 0, sizeof(z)); // 전부 0(= '\0')으로
    strcpy(z, "OK");
    printf("memset + strcpy: %s\n", z);
}

//---------------------------------------------
// 실습: 사용자 입력을 받아 안전하게 이어 붙인 후 검색/토큰화
//---------------------------------------------
void practice_io_concat_search(void)
{
    puts("\n[실습] 입력 → 안전 붙이기 → 검색/토큰화");

    char name[32];
    char hobby[64];
    char sentence[128] = { 0 }; // 결과 누적 버퍼

    input_line("이름을 입력: ", name, sizeof(name));
    input_line("취미를 입력(쉼표/공백으로 여러 개): ", hobby, sizeof(hobby));

    // 안전하게 이어붙이기 (직접 길이 체크)
    size_t cap = sizeof(sentence);
    size_t used = strlen(sentence);

    // "name=" 붙이기
    const char* prefix1 = "name=";
    size_t need = strlen(prefix1);
    if (used + need + 1 < cap) { strcat(sentence, prefix1); used += need; }
    if (used + strlen(name) + 1 < cap) { strcat(sentence, name); used = strlen(sentence); }

    // ", hobbies=" 붙이기
    const char* prefix2 = ", hobbies=";
    need = strlen(prefix2);
    if (used + need + 1 < cap) { strcat(sentence, prefix2); used += need; }
    if (used + strlen(hobby) + 1 < cap) { strcat(sentence, hobby); used = strlen(sentence); }

    printf("문장: \"%s\"\n", sentence);

    // 검색 예시: 'a'가 포함되어 있는지
    if (strchr(sentence, 'a')) {
        puts("→ 'a' 문자가 포함되어 있음");
    }

    // 부분 문자열 "name=" 위치
    const char* pos = strstr(sentence, "name=");
    if (pos) {
        printf("→ \"name=\" 시작 인덱스: %td\n", (ptrdiff_t)(pos - sentence));
    }

    // 취미 토큰화(원본 훼손되므로 복사본에서)
    char hobbies_copy[64];
    strcpy(hobbies_copy, hobby);
    printf("취미 토큰들:");
    for (char* t = strtok(hobbies_copy, " ,;"); t; t = strtok(NULL, " ,;")) {
        printf(" [%s]", t);
    }
    puts("");
}

//---------------------------------------------
// main: 각 데모 실행
//---------------------------------------------
int main(void)
{
    puts("C <string.h> 수업용 종합 데모 시작");

    demo_strlen();
    demo_copy_concat();
    demo_compare();
    demo_search();
    demo_tokenize();
    demo_memory_block();
    practice_io_concat_search();

    puts("\n끝. 질문: 어디서 터질 것 같은지/왜 그런지 말해보자.");
    return 0;
}