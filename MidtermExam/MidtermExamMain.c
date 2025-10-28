/*
    main.c — "LG 선수 상태 CSV 저장 → 다시 읽어 출력" (MSVC / C언어 복습용 요약노트)
    ------------------------------------------------------------------------------
    이 파일은 중간고사 대비 이론 총정리를 "코드 주석"으로 학습하도록 설계했다.
    오늘 수업 범위는 다음을 **실제로 사용**해서 동작 확인한다.

    [오늘 버전에서 실제 사용하는 것]
      - 표준/secure I/O: printf, scanf_s, fprintf, fgets, fopen_s, fclose
      - 단문 입력 제어: getchar (개행 소비/일시정지), getch (에코 없이 1키 입력)
      - 문자열: strtok_s(토크나이즈), strcpy_s, strlen, strcmp 등
      - 숫자 변환: strtol, strtod
      - 배열/구조체: struct Player, 고정 배열
      - Windows 지연/대기: Sleep(ms)
      - ANSI Escape: 색상 및 간단한 꾸밈 (Windows 10+ 에서 콘솔 모드 설정 필요할 수 있음)

    [유의]
      - 본 코드는 **MSVC(Visual Studio)** 기준으로 작성되었고, secure 함수 계열을 사용한다.
      - scanf_s("%s", buf, (rsize_t)buf_len)처럼 **문자열 입력에는 반드시 버퍼 크기를 같이 전달**해야 한다.
      - 오늘은 "고정 크기 배열(최대 128명)"로 제한한다. 동적할당은 다음 시간 확장.
      - 한글 이름을 CSV로 저장할 때, 콘솔 코드페이지/파일 인코딩 문제로 문자열이 깨질 수 있다.
        *채점/수업 편의상 UTF-8(Without BOM)로 취급하고, 콘솔 출력이 깨질 수 있다는 점만 주석으로 인지.*

    빌드: MSVC (예: cl /W4 /utf-8 main.c)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strtok_s, strcpy_s, strlen, strcmp 등
#include <windows.h>   // Sleep, 콘솔 모드 설정(ANSI 활성용)
#include <conio.h>     // getch

// -----------------------------------------------
// ANSI Escape 간단 매크로 (Windows 10 이상에서 권장)
// -----------------------------------------------
#define ESC         "\x1b"
#define CSI         ESC "["     // Control Sequence Introducer
#define CLR_RESET   CSI "0m"
#define FG_RED      CSI "31m"
#define FG_GREEN    CSI "32m"
#define FG_YELLOW   CSI "33m"
#define FG_BLUE     CSI "34m"
#define FG_MAGENTA  CSI "35m"
#define FG_CYAN     CSI "36m"
#define FG_WHITE    CSI "37m"
#define BOLD        CSI "1m"

// Windows 콘솔에서 ANSI 이스케이프를 사용하려면 "Virtual Terminal Processing"을 켜야 한다.
// 실패해도 프로그램은 동작하나, 색상/꾸밈은 안 보일 수 있다.
static void enable_ansi_if_possible(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;

    mode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, mode);
}

// -----------------------------------------------
// 도메인 모델
// -----------------------------------------------
typedef struct {
    char   name[64];   // 선수 이름 (공백 없는 입력 권장; 공백 입력 처리하려면 fgets + 개행 제거 등으로 확장)
    int    age;        // 나이 (양의 정수)
    int    number;     // 등번호 (양의 정수)
    double avg;        // 타율 0.000 ~ 0.999
} Player;

#define MAX_PLAYERS 128

// -----------------------------------------------
// 간단 유틸: 입력 버퍼(개행 등) 비우기
// -----------------------------------------------
static void flush_stdin_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* 버림 */ }
}

// -----------------------------------------------
// 1) 사용자 입력으로 Player 배열 채우기 → CSV 파일로 저장
//    - scanf_s: 안전한 문자열/숫자 입력
//    - fopen_s/fprintf: 파일 쓰기
//    - sprintf_s: 안전한 포맷 문자열 구성(주로 디버그용/설명용)
// -----------------------------------------------
static int write_players_to_csv_interactive(const char* csv_path)
{
    Player players[MAX_PLAYERS];
    int n = 0;

    printf(FG_CYAN BOLD "[입력 단계]" CLR_RESET " 저장할 선수 수를 입력하세요 (최대 %d): ", MAX_PLAYERS);
    if (scanf_s("%d", &n) != 1 || n < 1 || n > MAX_PLAYERS) {
        printf(FG_RED "잘못된 개수입니다. 프로그램을 종료합니다.\n" CLR_RESET);
        flush_stdin_line();
        return -1;
    }
    flush_stdin_line(); // 개행 제거

    for (int i = 0; i < n; ++i) {
        printf(FG_YELLOW "\n[%d/%d] 선수 정보 입력\n" CLR_RESET, i + 1, n);

        // 이름 입력: 공백 없는 문자열 기준(공백 처리하려면 fgets로 변경 권장)
        // scanf_s로 문자열 입력 시에는 반드시 버퍼 크기를 함께 전달해야 함.
        // 예: scanf_s("%63s", players[i].name, (rsize_t)sizeof(players[i].name));
        printf("  이름(공백 없이): ");
        if (scanf_s("%63s", players[i].name, (rsize_t)sizeof(players[i].name)) != 1) {
            printf(FG_RED "  입력 오류(이름)\n" CLR_RESET);
            flush_stdin_line();
            --i; // 재입력 유도
            continue;
        }

        printf("  나이(양의 정수): ");
        if (scanf_s("%d", &players[i].age) != 1 || players[i].age <= 0) {
            printf(FG_RED "  입력 오류(나이)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        printf("  등번호(양의 정수): ");
        if (scanf_s("%d", &players[i].number) != 1 || players[i].number <= 0) {
            printf(FG_RED "  입력 오류(등번호)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        printf("  타율(0.000 ~ 0.999, 소수3자리 권장): ");
        if (scanf_s("%lf", &players[i].avg) != 1 || players[i].avg < 0.0 || players[i].avg >= 1.0) {
            printf(FG_RED "  입력 오류(타율)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        flush_stdin_line(); // 각 항목 입력 후 남은 개행 정리
    }

    // 파일 열기 (쓰기)
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, csv_path, "w");
    if (err != 0 || fp == NULL) {
        printf(FG_RED "파일 열기 실패(쓰기): %s  (errno=%d)\n" CLR_RESET, csv_path, err);
        return -2;
    }

    // CSV 헤더
    // CSV는 콤마(,) 구분, 소수점(.) 사용. 채점기와 합의된 고정 헤더를 사용한다.
    fprintf(fp, "name,age,number,avg\n");

    for (int i = 0; i < n; ++i) {
        // 한글 이름을 그대로 기록(UTF-8 가정). 콘솔 출력은 깨질 수 있음.
        // 숫자 포맷은 소수점 3자리 고정(채점 오차 방지). 필요 시 자리수/반올림 규칙을 고정한다.
        fprintf(fp, "%s,%d,%d,%.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }

    fclose(fp);

    printf(FG_GREEN "\n[저장 완료] %d명 정보를 CSV로 기록했습니다 → %s\n" CLR_RESET, n, csv_path);

    // getchar: **라인 일시정지** 용도 (개행 소비 후 잠깐 멈추고 싶을 때 적합)
    printf("엔터를 누르면 CSV를 다시 읽어옵니다...");
    getchar(); // 위에서 flush_stdin_line으로 정리했기 때문에 여기서는 실제 대기

    return n; // 기록한 개수 반환
}

// -----------------------------------------------
// 2) CSV 읽어서 Player 배열 로딩 → 표 형태 출력
//    - fgets: 한 줄씩 안전하게 읽기
//    - strtok_s: 콤마 구분 토큰 분해(멀티스레드 안전 버전; 내부 상태 포인터를 외부로 분리)
//    - strtol/strtod: 문자열 → 숫자 변환
//    - Sleep: 표 출력 속도 조절(피드백 연출)
//    - getch: "아무 키나" 즉시 입력(에코 없음)
// -----------------------------------------------
static int read_players_from_csv_and_print(const char* csv_path)
{
    Player players[MAX_PLAYERS];
    int count = 0;

    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, csv_path, "r");
    if (err != 0 || fp == NULL) {
        printf(FG_RED "파일 열기 실패(읽기): %s  (errno=%d)\n" CLR_RESET, csv_path, err);
        return -1;
    }

    char line[512];
    int line_no = 0;

    while (fgets(line, sizeof(line), fp)) {
        ++line_no;

        // 개행 제거(있으면)
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            // CRLF 모두 고려
            while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
                line[--len] = '\0';
            }
        }

        // 헤더 라인 건너뛰기
        if (line_no == 1) {
            // 기대 헤더: name,age,number,avg
            // 검사해도 되고, 단순히 스킵해도 된다. 여기선 간단 스킵.
            continue;
        }

        // 토크나이즈
        char* ctx = NULL; // strtok_s의 내부 상태 포인터(스레드 안전을 위해 외부 변수 사용)
        char* tok = NULL;

        tok = strtok_s(line, ",", &ctx);
        if (!tok) continue;
        char name_buf[64] = { 0 };
        strcpy_s(name_buf, sizeof(name_buf), tok);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        char* endp = NULL;
        long age_val = strtol(tok, &endp, 10);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        long number_val = strtol(tok, &endp, 10);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        double avg_val = strtod(tok, &endp);

        if (count < MAX_PLAYERS) {
            strcpy_s(players[count].name, sizeof(players[count].name), name_buf);
            players[count].age = (int)age_val;
            players[count].number = (int)number_val;
            players[count].avg = avg_val;
            ++count;
        }
    }

    fclose(fp);

    // 표 출력
    printf(FG_MAGENTA BOLD "\n[로드/출력 단계] CSV에서 %d명 로드됨 → 표로 출력합니다.\n" CLR_RESET, count);
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);
    printf(BOLD FG_WHITE "%-16s %-6s %-8s %-8s\n" CLR_RESET, "NAME", "AGE", "NUMBER", "AVG");
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);

    for (int i = 0; i < count; ++i) {
        // Sleep: 시각적 피드백을 위한 지연 (실제 로직과 무관)
        Sleep(50);

        // 타율은 소수점 3자리로 고정 출력 (채점 편차 최소화)
        printf("%-16s %-6d %-8d %-8.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);

    // getch: 에코 없이 즉시 키 입력 하나를 읽는다.
    // - scanf/gets/fgets와 달리 콘솔에 글자가 찍히지 않는다.
    // - "아무 키나 누르면 계속" UX에 적합.
    printf(FG_GREEN "\n출력을 모두 완료했습니다. " CLR_RESET);
    printf("아무 키나 누르면 종료합니다...");
    (void)_getch();

    return count;
}

// -----------------------------------------------
// main
//   - ANSI 활성(가능한 경우)
//   - 단계 1: 사용자 입력 → CSV 저장 (write_players_to_csv_interactive)
//   - 단계 2: CSV 읽기 → 표 출력 (read_players_from_csv_and_print)
//   - 각 단계에서 printf/scanf_s/getchar/getch/Sleep/문자열/파일I/O를 실사용
// -----------------------------------------------
int main(void)
{
    enable_ansi_if_possible();

    const char* CSV_PATH = "players_lg.csv";

    printf(FG_BLUE BOLD "=== LG 선수 상태 CSV 저장/읽기 실습 ===\n" CLR_RESET);
    printf("이름/나이/등번호/타율을 입력 받아 CSV로 저장하고, 다시 읽어서 표로 출력합니다.\n");
    printf("문자열 입력은 공백 없는 이름을 권장합니다. (공백 지원은 다음 시간에 fgets로 확장)\n\n");

    int wrote = write_players_to_csv_interactive(CSV_PATH);
    if (wrote < 0) {
        printf(FG_RED "입력/저장 단계에서 오류가 발생했습니다. 프로그램을 종료합니다.\n" CLR_RESET);
        return 1;
    }

    int loaded = read_players_from_csv_and_print(CSV_PATH);
    if (loaded < 0) {
        printf(FG_RED "읽기/출력 단계에서 오류가 발생했습니다.\n" CLR_RESET);
        return 2;
    }

    // 마무리
    printf("\n" FG_CYAN "실습 종료. 수고했다. (파일: %s)\n" CLR_RESET, CSV_PATH);
    return 0;
}

/*
 [학습 포인트 요약]

 1) printf / scanf_s / getchar / getch
    - printf: 콘솔로 문자열 출력. 서식(%d, %f, %s 등) 사용.
    - scanf_s: 콘솔에서 값을 **안전하게** 입력. 문자열 입력 시 버퍼 크기 필수.
      예) char name[64]; scanf_s("%63s", name, (rsize_t)sizeof(name));
    - getchar: 표준 입력에서 **문자 1개**를 읽는다. 주로 "개행 소비"나 "엔터 대기" 용도로 사용.
    - getch: 콘솔에서 **에코 없이 1키**를 즉시 읽는다. (Windows 전용, <conio.h>)

 2) 파일 입출력 (MSVC secure 계열)
    - fopen_s(&fp, "path", "mode"): 파일 열기. 성공 시 0, 실패 시 errno 반환.
    - fprintf(fp, "...", ...): 파일에 서식 있는 텍스트 쓰기.
    - fgets(buf, n, fp): 한 줄 읽기. 최대 n-1 글자, 개행 포함될 수 있음.
    - fclose(fp): 파일 닫기.

 3) 문자열 처리
    - strtok_s(line, ",", &ctx): 구분자(여기선 콤마) 기준으로 토큰 분리.
      *strtok과 달리 내부 상태를 외부 변수(ctx)에 보관하므로 **스레드 안전성/중첩 파싱**에 유리.
    - strcpy_s(dst, dst_size, src): 안전한 문자열 복사(버퍼 크기 체크).
    - strlen, strcmp: 길이/비교.

 4) 숫자 변환
    - strtol(tok, &endp, 10): 문자열 → long (10진수). 변환 실패 시 endp==tok 등으로 체크.
    - strtod(tok, &endp): 문자열 → double.

 5) 구조체/배열
    - struct Player { name, age, number, avg };
    - 고정 크기 배열 Player players[MAX_PLAYERS]; 로 메모리를 관리(오늘 버전).

 6) Sleep / ANSI
    - Sleep(ms): ms 밀리초 동안 스레드 일시정지. 출력 템포/프로그레스 표현 등에 사용.
    - ANSI 이스케이프: "\x1b[31m"(빨강), "\x1b[0m"(리셋) 등. Windows 10+ 에서 콘솔 모드 설정 필요할 수 있음.
      enable_ansi_if_possible()에서 Virtual Terminal Processing 켠다.

 [확장 아이디어(다음 시간)]
    - 이름 입력을 fgets로 받고, 끝 개행 제거 + 공백 허용.
    - 동적할당으로 선수 수 제한 해제 (malloc/realloc/free).
    - 정렬(타율 내림차순, 이름 오름차순), 필터(age>=X).
    - manifest.txt에 레코드 수/체크섬 기록 → 자동 채점 기반.
*/