// Visual Studio 2022 x86 Release Mode
// Line_Rectangle.cpp

#include "pch.h"			// 컴파일 속도 향상
#define _USE_INIT_WINDOW_	// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"		// EasyWin32 사용

NOT_USE_MESSAGE

void InitWindow()
{
	gp_window_title = "라인을 그려 사각형 그리기";
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

int main()
{
	// 클라이언트 영역 조정
	ChangeWorkSize(500, 300);
	// 굵기가 3인 파란색 선 속성 사용
	SelectPenObject(RGB(0, 0, 255), 3);
	// 선을 네 번 그려 사각형을 만듦
	Line(10, 10, 100, 10);
	Line(100, 10, 100, 100);
	Line(100, 100, 10, 100);
	Line(10, 100, 10, 10);

	// 굵기가 3, 점선, 색상이 빨간색 선 속성 사용
	// 선 굵기 2 이상 -> PS_GEOMETRIC 속성 함께 사용
	SelectPenObject(RGB(255, 0, 0), 3, PS_DOT | PS_GEOMETRIC);
	// 선 그리기 기준점 설정
	MoveTo(150, 10);
	// 선을 네 번 그려 사각형 만듦
	Line(150, 10, 250, 10);
	Line(250, 10, 250, 100);
	Line(250, 100, 150, 100);
	Line(150, 100, 150, 10);
	
	// 정보를 윈도우에 출력
	ShowDisplay();
	return 0;
}