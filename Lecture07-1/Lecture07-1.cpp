#include <stdio.h>
#include <stdlib.h> // malloc, calloc, realloc, free

/*
  [오늘의 목표]
  1. 스택(Stack)과 힙(Heap) 차이를 눈으로 확인해본다.
  2. malloc / calloc / realloc / free 를 써본다.
  3. 동적 메모리를 만들고, 늘리고, 지우는 연습을 한다.

  ※ 헷갈리지 말자!
     - Stack: 함수 안에서 잠깐 생겼다가 사라지는 공간
     - Heap: 우리가 직접 malloc() 으로 "빌려서" 쓰는 공간
*/

int main()
{
    printf("=== 1단계: Stack(스택)과 Heap(힙) 차이 ===\n");

    // [1] 스택: 함수가 끝나면 자동으로 사라지는 변수
    int a = 10;
    printf("스택 변수 a의 값: %d\n", a);
    printf("스택 변수 a의 주소: %p\n", &a);

    // [2] 힙: malloc으로 만든 변수 (직접 free 해야 함!)
    int* b = (int*)malloc(sizeof(int)); // int 1개 공간 빌리기
    *b = 20; // 값 저장
    printf("힙 변수 b의 값: %d\n", *b);
    printf("힙 변수 b의 주소: %p\n", b);

    // [3] free: 빌린 공간은 꼭 돌려주기!
    free(b);
    printf("(free 이후 b는 더 이상 쓸 수 없음!)\n\n");


    printf("=== 2단계: malloc과 calloc의 차이 ===\n");

    // malloc: 공간만 만들고 안에 뭐가 들어있는지는 모름 (쓰레기값일 수도 있음)
    int* arr1 = (int*)malloc(sizeof(int) * 5);
    // calloc: 공간을 만들고 안을 0으로 채워줌
    int* arr2 = (int*)calloc(5, sizeof(int));

    printf("malloc으로 만든 arr1:\n");
    for (int i = 0; i < 5; i++)
        printf("arr1[%d] = %d\n", i, arr1[i]);

    printf("\ncalloc으로 만든 arr2:\n");
    for (int i = 0; i < 5; i++)
        printf("arr2[%d] = %d\n", i, arr2[i]);

    // 값 직접 넣기
    for (int i = 0; i < 5; i++)
        arr1[i] = i + 1;

    printf("\narr1에 값 채운 후:\n");
    for (int i = 0; i < 5; i++)
        printf("arr1[%d] = %d\n", i, arr1[i]);

    free(arr1);
    free(arr2);
    printf("(arr1, arr2 해제 완료)\n\n");


    printf("=== 3단계: realloc (공간 늘리기) ===\n");

    // 3칸짜리 int 배열 만들기
    int* nums = (int*)malloc(sizeof(int) * 3);
    for (int i = 0; i < 3; i++)
        nums[i] = i + 1;
    printf("원래 배열 (3칸): ");
    for (int i = 0; i < 3; i++)
        printf("%d ", nums[i]);
    printf("\n");

    // realloc으로 6칸으로 늘리기
    nums = (int*)realloc(nums, sizeof(int) * 6);
    for (int i = 3; i < 6; i++)
        nums[i] = (i + 1) * 10;

    printf("늘린 배열 (6칸): ");
    for (int i = 0; i < 6; i++)
        printf("%d ", nums[i]);
    printf("\n");

    free(nums);
    printf("(nums 해제 완료)\n\n");


    printf("=== 4단계: 잘못된 free 예시 (이건 하면 안됨!) ===\n");
    int* x = (int*)malloc(sizeof(int));
    *x = 123;
    printf("x의 값: %d\n", *x);
    free(x); // 여기까지는 정상
    // free(x); // ❌ 두 번 하면 프로그램 터짐 (double free)
    // printf("%d\n", *x); // ❌ 이미 해제된 메모리 접근 금지
    x = NULL; // ✅ 안전하게 NULL로 바꿔두기
    printf("free 후 NULL로 변경 완료!\n");

    printf("\n=== 프로그램 종료 ===\n");
    return 0;
}