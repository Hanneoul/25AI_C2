// Lecture13_main_D.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
/* main_dll_dynamic.c
 *
 * 이 프로그램은 런타임에 DLL을 동적으로 로드한다.
 * - 실행 중에 DLL 파일을 교체하면(같은 함수 시그니처 지키면) 동작이 달라짐.
 * - 교육 포인트: DLL만 교체하면 프로그램을 다시 빌드할 필요 없음.
 *
 * 이 예제에서는 Roll 함수와 who_am_i 함수를 GetProcAddress로 얻어 호출한다.
 */

#include <stdio.h>
#include <windows.h>
#include "fun.h" /* 함수 시그니처(컴파일 시 타입정보용) */

 /* 함수 포인터 타입 정의 — 정확히 헤더와 맞춰야 함 */
typedef int (*roll_fn_t)(void);
typedef const char* (*who_fn_t)(void);

int main(void) {
    printf("=== DLL dynamic load demo ===\n");

    /* DLL 파일 이름(실행파일과 동일 폴더에 있어야 함) */
    const char* dllname = "mylib.dll"; /*  이 이름을 바꾸도록 시킬 수 있음 */

    HMODULE h = LoadLibraryA(dllname);
    if (!h) {
        DWORD err = GetLastError();
        printf("Failed to load %s (error %lu). Make sure the DLL is next to the exe.\n", dllname, err);
        return 1;
    }

    /* GetProcAddress로 심볼(함수) 얻기 — 이름은 C 스타일(맹글링 없음) */
    roll_fn_t roll_fn = (roll_fn_t)GetProcAddress(h, "roll");
    who_fn_t who_fn = (who_fn_t)GetProcAddress(h, "who_am_i");

    if (!roll_fn || !who_fn) {
        printf("Failed to find required functions in %s. Did you export them correctly?\n", dllname);
        FreeLibrary(h);
        return 1;
    }

    printf("Who am I? %s\n", who_fn());

    for (int i = 0; i < 5; ++i) {
        int r = roll_fn();
        printf("Roll %d -> %d\n", i + 1, r);
    }

    FreeLibrary(h); /* DLL 언로드 */
    return 0;
}
