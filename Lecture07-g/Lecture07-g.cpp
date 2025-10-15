#include <stdio.h>
#include <stdlib.h>

int main()
{
	int* ES_Maum;
	ES_Maum = (int*)malloc(sizeof(int)*2);
	
	/* [0] 좋아하는 성별
	 *     0: 여자
	 *     1: 남자
	 *     2: 기타
	 * [1] 애인 유무
	 *     0: 없음
	 *     1: 있음
	 */
	ES_Maum[0] = 0;
	ES_Maum[1] = 1;

	if (ES_Maum[0] == 1)
	{
		printf("그는 게이였다.\n");
	}
	else if (ES_Maum[0] == 0)
	{
		printf("그는 여성을 사랑한다\n");
	}
	else if (ES_Maum[0] == 0)
	{
		printf("그는... 하아...\n");
	}

	if (ES_Maum[1] == 1)
	{
		printf("그는 사랑꾼이다.\n");
	}
	else if (ES_Maum[1] == 0)
	{
		printf("그는 외롭다.\n");
	}

	free(ES_Maum);
	return 0;
}
