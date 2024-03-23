// Visual Studio 2022 x86 Release Mode
// math 헤더 파일 -> sin,cos 그래프 그리기
// Make_Sin_Cos_Graph.cpp

#include "pch.h"
#include <math.h>
#define _USE_INIT_WINDOW_
#include "tipsware.h"

#define RADIAN  3.141592 / 180

// 이 프로그램은 특별한 메시지를 사용하지 않음
NOT_USE_MESSAGE

// 윈도우 전역 속성 사용
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "sin_cos 함수 그리기";
	// 윈도우 속성 변경(제목 속성, 창 메뉴 x)
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 프로그램 실행하는 메인 함수
int main()
{
	ChangeWorkSize(720, 200);
	// 회색 점선을 사용해서 x축과 y축을 의미하는 선을 그림
	Line(0, 100, 720, 100, RGB(200, 200, 200), 1, PS_DOT);
	// 수직선(y축) -> (360 => x축 중심 좌표)
	Line(360, 0, 360, 200);
	// 파란색, 굵기3인 선 속성 사용
	SelectPenObject(RGB(0, 0, 255), 3);
	// sin 그래프 시작 위치 정함
	MoveTo(0, 100);
	int y;
	// 0도 ~ 720도까지 반복
	for (int degree = 0; degree < 720; ++degree)
	{
		// degree에 해당하는 sin값 구하고 sin 값을 100배 증가 시켜 사용
		y = 100 - (int)(sin(degree * RADIAN) * 100);
		// 기준점(degree, y) 좌표까지 선을 그림
		// 지금 그린 이 끝 점 -> 다시 다음 그리기의 기준점이 됨
		LineTo(degree, y);
	}

	// 빨간색, 굵기3인 속 속성 사용
	SelectPenObject(RGB(255, 0, 0), 3);
	// cos 그래프 시작 위치 정함
	MoveTo(0, 0);
	// 0도 ~ 720도까지 반복
	for (int degree = 0; degree < 720; ++degree)
	{
		// degree에 해당하는 cos값 구하고 cos 값을 100배 증가시킴
		y = 100 - (int)(cos(degree * RADIAN) * 100);
		// 기준점에서 (degree, y)좌표까지 선을 그림
		LineTo(degree, y);
	}
	ShowDisplay(); // 정보를 윈도우에 출력
	return 0;
}