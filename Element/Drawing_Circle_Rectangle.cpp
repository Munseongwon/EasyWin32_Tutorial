// Visual Studio 2022 x86 Release Mode
// Drawing_Circle_Line
// Drawing_Circle_Rectangle.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 윈도우 전역 속성 초기화 함수 직접 지정
#include "tipsware.h"			// EasyWin32 사용

// 이 프로그램은 특별한 메시지를 사용하지 않음
NOT_USE_MESSAGE

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "원 그리기 & 사각형 그리기";
	// 윈도우 속성 변경
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 프로그램 실행하는 메인 함수
int main()
{
	// x축 방향으로 20개의 녹색 사각형을 그림
	for (int i = 0; i < 20; ++i) {
		Rectangle(10 + i * 20, 10, 27 + i * 20, 30, RGB(0, 200, 0));
	}
	// x축 방향으로 20개의 녹색 타원을 그림
	for (int i = 0; i < 20; ++i) {
		Ellipse(10 + i * 20, 32, 27 + i * 20, 52, RGB(0, 200, 0), RGB(0, 255, 0));
	}
	// 테두리 색상을 파랑색으로 변경
	SelectPenObject(RGB(0, 0, 255));
	// 채우기 색상을 어두운 파랑색으로 변경
	SelectBrushObject(RGB(0, 0, 128));
	// x축 방향으로 20개의 녹색 사각형을 그림
	for (int i = 0; i < 20; ++i) {
		Rectangle(10 + i * 20, 54, 27 + i * 20, 74);
	}
	// 테두리 색상을 하늘색으로 변경
	SelectPenObject(RGB(0, 100, 255));
	// 시작점은 x,y축으로 모두 증가 -> 끝점은 모두 감소하는 형식
	for (int i = 0; i < 155; i += 4) {
		Ellipse(10 + i, 110 + i, 220 - i/3, 320 - i/3);
	}
	// 시작점은 x,y축으로 모두 증가 -> 끝점은 모두 감소하는 형식
	for (int i = 0; i < 100; i += 4) {
		Rectangle(310 + i, 110 + i, 520 - i, 320 - i);
	}
	// 정보를 윈도우에 출력
	ShowDisplay();
	// 프로그램 종료
	return 0;
}