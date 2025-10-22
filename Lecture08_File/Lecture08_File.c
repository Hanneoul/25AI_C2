/*
    file_io_example.c
    목적:
      - 특정 문자열을 파일에 쓰고(fopen+fprintf), 파일에서 다시 읽어오는(fopen+fgets) 기본 흐름을 보여준다.
      - 주석 안에 "컴퓨터 파일 구조/스트림/버퍼링/모드/에러처리" 개념을 상세히 정리했다.
        (요약표 따로 없음. 전부 주석에 몰아넣음)

    [컴퓨터 파일 & 스트림 개념 핵심 정리]

    1) 파일(File)과 파일 경로(Path)
       - 파일은 OS가 디스크(또는 유사 저장장치)에 유지하는 바이트 시퀀스.
       - 경로는 "어디에 있는지"를 가리킨다.
         * 상대경로: "example.txt" 처럼 현재 작업 디렉터리 기준.
         * 절대경로: "C:\\data\\example.txt" 또는 "/home/user/example.txt" 처럼 루트부터 전체.
       - 현재 작업 디렉터리는 실행 환경에 따라 다르다(IDE/터미널/OS 설정에 영향).

    2) C의 스트림(Stream)과 파일 포인터(FILE*)
       - C 표준 라이브러리는 디스크 파일을 "스트림"으로 다룬다. 스트림은 바이트 흐름의 추상화.
       - fopen(...)은 OS 파일 핸들을 감싼 FILE* (파일 포인터; 실제로는 제어구조체 포인터)를 반환.
       - FILE*는 해당 스트림의 상태(버퍼, 파일 위치, 에러/EOF 플래그 등)를 관리한다.
       - 파일 위치 지시자(file position indicator): ftell/fseek/rewind로 이동·조회 가능.

    3) 텍스트 모드 vs 바이너리 모드
       - 텍스트("r","w","a","r+","w+","a+") : 줄바꿈 변환이 있을 수 있다.
         * Windows: '\n'을 파일에 쓰면 실제로는 "\r\n" 저장될 수 있음(읽기 시 역변환).
         * POSIX(리눅스/맥): 대체로 차이 없음.
       - 바이너리("rb","wb","ab", ...) : 바이트 그대로 read/write. 줄바꿈 변환 없음.
       - 이 예제는 텍스트 모드("w","r")로 동작. 줄바꿈 변환이 있어도 fgets/printf로는 문제 없음.
         (이식성과 예측성을 더 중시할 땐 "wb"/"rb" 선택 후 fputs/fread/fwrite 사용 고려)

    4) 버퍼링(Buffering)
       - stdio는 성능을 위해 내부 버퍼를 둔다.
         * 전부버퍼링(full), 줄버퍼링(line), 비버퍼링(unbuffered) 등 환경/디바이스에 따라 다름.
       - 쓰기 시 버퍼에 쌓였다가 조건에 따라 실제 파일로 flush(배출)된다.
         * fclose(fp) 시 자동 flush. 필요 시 fflush(fp)로 강제 flush 가능.
       - setvbuf(...)로 버퍼링 전략을 바꿀 수 있지만 보통 기본값으로 충분.

    5) 파일 열기 모드("mode") 동작 요약
       - "w": 쓰기 전용. 파일이 있으면 길이를 0으로 잘라(초기화) 덮어씀. 없으면 새로 만듦.
       - "a": 쓰기 전용. 항상 파일 끝에 추가. 없으면 새로 만듦.
       - "r": 읽기 전용. 없으면 실패(NULL).
       - "+": 읽기도/쓰기(갱신)도 하고 싶을 때 (예: "r+","w+","a+").
       - "b": 바이너리 힌트(Windows 중요). POSIX에선 무시될 수 있음.
       - 주의: "w"는 기존 내용을 지워버린다. 기존 유지가 필요하면 "a"나 "r+"를 고려.

    6) 문자 인코딩(Encoding)
       - 소스 문자열이 UTF-8이라고 가정. 텍스트 모드로 쓰면 UTF-8 바이트가 그대로 저장된다.
       - Windows 메모장 등 일부 앱은 BOM 없는 UTF-8 표시가 애매할 수 있음(요즘은 대개 OK).
       - 크로스플랫폼 시 인코딩 일관성을 문서화해 두는 것이 안전.

    7) 읽기 함수 선택 가이드(현업에서 흔히 겪는 함정 요약)
       - fgets: 줄 단위 안전 읽기(버퍼 오버플로 방지). 권장.
       - fscanf: 서식 불일치/공백 처리로 예상 밖 동작 가능. 초보자에겐 난이도↑.
       - fgetc/fputc: 문자 단위 처리. 느리지만 제어가 쉬움.
       - fread/fwrite: 바이너리/고정 길이 처리에 적합(구조체 I/O 등).

    8) EOF/에러 처리
       - EOF는 "더 읽을 데이터가 없음"을 의미. fgets가 NULL이면 EOF 또는 에러.
       - 에러 여부는 ferror(fp)로 확인. 에러 메시지는 perror(...)로 즉시 진단 출력 가능.
       - 반드시 fopen 실패(NULL) 체크 → perror 또는 사용자 친화 메시지 후 종료.

    9) 안전한 사용 패턴(요지)
       - fopen → NULL 검사 → 작업 → 에러 시 즉시 처리 → 반드시 fclose.
       - 쓰기 후 즉시 읽으려면: (1) 같은 핸들에서 fseek/fflush/rewind로 위치와 버퍼 정리, 또는
                               (2) 닫고 다시 여는 편이 단순·명확(이 예제는 (2) 방식).

    10) 동시성/멀티프로세스 주의
       - 여러 프로세스/스레드가 같은 파일을 동시에 쓰면 레이스 발생 가능.
       - 잠금(lock)이나 원자적 append, 임시파일+rename 전략 등을 사용.
       - fsync(비표준; POSIX)는 커널 버퍼→디스크까지 강제 동기화. 표준 C는 보장하지 않음.

    -------------------------------------------------------------
    빌드:  gcc -Wall -Wextra -O2 file_io_example.c -o file_io_example
    실행:  ./file_io_example
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>   // perror 사용 시 필요(선택)

// 안전한 상수 크기 버퍼
#define LINE_CAP 256



int main(void)
{
    const char* filename = "example.txt";    // 상대경로(현재 작업 디렉터리 기준)
    const char* line1 = "Hello, C File I/O!";
    const char* line2 = "두 번째 줄입니다. UTF-8 한글도 그대로 저장됩니다.";

    // ---------------------------
    // 1) 쓰기 단계: "w" (텍스트 모드)
    //    - 기존 파일이 있다면 내용을 싹 지우고 새로 씀(트렁케이트).
    //    - 기존 내용 유지가 목적이면 "a" 또는 "r+"를 고려.
    // ---------------------------
    FILE* fp = NULL;
    fopen_s(&fp, filename, "w");
    if (fp == NULL) {
        // fopen 실패 원인은 errno에 저장됨. perror는 errno를 해석해 사람이 읽을 수 있게 출력.
        perror("파일 열기 실패(쓰기 모드)");  // 예: 권한 없음, 경로 없음 등
        return EXIT_FAILURE;
    }

    // fprintf는 printf의 파일 버전. 텍스트 모드에서는 '\n'이 플랫폼 규칙에 맞춰 저장될 수 있음.
    if (fprintf(fp, "%s\n", line1) < 0) {
        perror("파일 쓰기 실패(line1)");
        fclose(fp); // 열린 건 닫고 종료
        return EXIT_FAILURE;
    }
    if (fprintf(fp, "%s\n", line2) < 0) {
        perror("파일 쓰기 실패(line2)");
        fclose(fp);
        return EXIT_FAILURE;
    }

    // 선택) 지금 당장 디스크에 밀어넣고 싶다면 fflush(fp)를 호출할 수 있음.
    // 하지만 어차피 닫으면서 flush 되므로 생략 가능.
    if (fclose(fp) == EOF) {
        perror("파일 닫기 실패(쓰기 단계)");
        return EXIT_FAILURE;
    }

    // ---------------------------
    // 2) 읽기 단계: "r" (텍스트 모드)
    //    - 파일이 존재해야 함. 없으면 NULL(실패).
    //    - fgets로 라인 단위 안전하게 읽는다(버퍼 오버런 방지).
    // ---------------------------
    fopen_s(&fp, filename, "r");
    if (fp == NULL) {
        perror("파일 열기 실패(읽기 모드)");
        return EXIT_FAILURE;
    }

    char buffer[LINE_CAP];
    int  line_no = 0;

    // fgets는 최대 (크기-1) 글자를 읽고, 항상 널('\0')로 끝맺음.
    // 개행 포함하여 읽으며, EOF 또는 에러면 NULL 반환.
    printf("=== 파일에서 읽은 내용(%s) ===\n", filename);
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // 출력 시, buffer 안의 개행 문자를 그대로 보여준다.
        printf("%2d: %s", ++line_no, buffer);
        // 만약 줄 끝 개행 제거가 필요하다면, 개행 탐색 후 '\0'로 치환하면 됨(여기선 그대로 둔다).
    }

    //fscanf

    if (ferror(fp)) {
        // EOF가 아니라 에러로 루프가 종료된 경우
        perror("\n읽기 중 에러 발생");
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (fclose(fp) == EOF) {
        perror("\n파일 닫기 실패(읽기 단계)");
        return EXIT_FAILURE;
    }

    printf("\n작업 완료.\n");
    return EXIT_SUCCESS;
}