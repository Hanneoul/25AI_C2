#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// 정수를 문자열로 변환하는 함수 (itoa 대체)
void int_to_str(int num, char* out) {
    char temp[32];
    int i = 0, j = 0;
    int neg = 0;

    if (num == 0) {
        out[0] = '0';
        out[1] = '\0';
        return;
    }

    if (num < 0) {
        neg = 1;
        num = -num;
    }

    // 숫자를 거꾸로 temp에 저장
    while (num > 0) {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (neg) temp[i++] = '-';
    temp[i] = '\0';

    // 뒤집기
    for (j = 0; j < i; j++)
        out[j] = temp[i - j - 1];
    out[i] = '\0';
}

void my_printf(const char* fmt, ...) {
    char buf[512] = { 0 };
    char numbuf[64];
    size_t pos = 0;

    va_list args;
    va_start(args, fmt);

    for (size_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i + 1] == 'd') {
            int n = va_arg(args, int);
            int_to_str(n, numbuf);
            strcat_s(buf, numbuf);
            i++; // %d 건너뛰기
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 's') {
            char* str = va_arg(args, char*);
            strcat_s(buf, str);
            i++;
        }
        else {
            size_t len = strlen(buf);
            buf[len] = fmt[i];
            buf[len + 1] = '\0';
        }
    }

    va_end(args);
    puts(buf); // printf 대신 puts 사용 (개행 자동 추가)
}

int main(void) {
    my_printf("이름: %s", "HG 이은석");
    my_printf("나이: %d", 25);
    my_printf("매력: %d, 분노: %d", 99, 9999999);
    my_printf("호감도: %d", -12345);
    return 0;
}