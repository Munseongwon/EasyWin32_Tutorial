// Visual Studio 2022 x86 Release Mode
// Print_GuGuDan
// GuGuDan.cpp

#include "pch.h"
#define _USE_INIT_WINDOW_
#include "tipsware.h"

NOT_USE_MESSAGE

void InitWindow()
{
	gp_window_title = "구구단 출력하기";
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

int main()
{
	// 2단 ~ 9단까지 출력
	for (int dan = 2; dan <= 9; ++dan) {
		// 각 단의 1 ~ 9까지 출력
		for (int i = 1; i <= 9; ++i) {
			// 단이 증가 -> x방향으로 이동하기 위해 x좌표에 dan 사용
			// 각 단에서 곱하는 값 증가 -> 아래쪽으로 출력하기 위해 y좌표에 i 사용
			printf(10 + (dan-2)*90, 10 + 20 * i, "%d x %d = %d", dan, i, dan * i);
		}
	}
	ShowDisplay(); // 정보를 윈도우에 출력
	return 0;
}