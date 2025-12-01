/* static_lib.c
 *
 * 정적 라이브러리 버전
 * 이 구현은 '장난스럽게' 항상 3을 반환한다 — predictable(정적)의 예.
 *
 * 컴파일해서 staticlib.lib 를 만들고, main_static.c 와 링크한다.
 */
 /*
:: 1) 컴파일 정적 라이브러리 소스 파일을 오브젝트로 만든다
cl / c / WX static_lib.c
:: / c = 컴파일만(링크 안 함), / WX = 경고를 에러로 취급(선택)

:: 2).lib(정적 라이브러리) 생성
lib / OUT:staticlib.lib static_lib.obj

:: 3) main(정적 링크) 컴파일& 링크(staticlib.lib를 링크)
cl main_static.c staticlib.lib

:: 4) 실행
main_static.exe
*/

#include "fun.h"

 /* roll: 정적 라이브러리는 "같은 결과"를 주는 느낌을 보여주기 위해
  * 항상 3을 반환하도록 만듦 
  */
int roll(void) {
    return 3; /* predictable, boring, but 교육적 */
}

/* who_am_i: 정적 라이브러리의 이름을 알려주는 간단한 문자열(읽기 전용)
 * 주의: 문자열은 라이브러리(.lib)에 포함되어도 안전함(정적 연결이므로).
 */
const char* who_am_i(void) {
    return "StaticLib: I am predictable.";
}
