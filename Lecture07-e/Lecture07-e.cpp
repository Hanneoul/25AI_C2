#include <stdio.h>
#include <stdlib.h>

int main()
{
	int arr[4] = { 4,3,2,1 };
	//크기가 4인 배열 선언
	int* pArr;
	//int 타입을 가리키는 포인터 pArr 선언     
	//int 타입사이즈 * 4 만큼의 메모리를 할당해라!    
	pArr = (int *)malloc(sizeof(int) * 4);
	if(pArr == NULL)
	{
		//메모리 할당에 실패하면 이쪽으로 들어옵니다.        
		printf("malloc error");
	}
	
	pArr = arr;
	
	for (int i = 0; i < 4; ++i)
	{
		printf("%d \n", pArr[i]);
	}

	//할당한 메모리를 해제해줍니다.    
	free(pArr);

	return 0; 
}



