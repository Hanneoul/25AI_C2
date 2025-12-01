/* dll_lib.c
 *
 * DLL 버전: 랜덤한 주사위(실제 rand())를 쓰거나 다른 전략을 쓸 수 있음.
 * 이 파일을 DLL로 빌드하면 main에서는 DLL만 교체해서 실행 결과가 달라짐.
 *
 * 주의: BUILDING_DLL 매크로는 game-style 헤더에서 export/import 처리할 때 사용하진 않지만,
 * MSVC에서 /LD로 빌드하면 자동으로 export 된다. (간단히 설명하려고 여기선 정의 안 함)
 */

/*:: 1) DLL 컴파일 & 링크 (mylib.dll 생성)
cl /LD dll_lib.c /Femylib.dll
:: /LD = DLL 생성, /Fe = 출력 이름 지정(여기선 mylib.dll)

:: 2) main_dll_dynamic 빌드
cl main_dll_dynamic.c

:: 3) 실행
main_dll_dynamic.exe
*/

#include "fun.h"
#include <stdlib.h>
#include <time.h>

 /* DLL 초기화용 함수(명시적 호출 없이도 rand 초기화 그림) */
static int initialized = 0;

static void ensure_init(void) {
    if (!initialized) {
        srand((unsigned)time(NULL)); /* 랜덤 시드(매번 다르게) */
        initialized = 1;
    }
}

/* roll: 실제 랜덤 주사위를 반환 */
int roll(void) {
    ensure_init();          /* rand 초기화 보장 */
    return (rand() % 6) + 1;/* 1..6 반환 */
}

/* who_am_i: DLL이면 자기소개 다르게 함 */
const char* who_am_i(void) {
    return "DLL: I am dynamic and random.";
}
