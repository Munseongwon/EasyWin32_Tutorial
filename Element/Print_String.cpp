// Visual Studio 2022 x86 Release Mode
// EasyWin32 Tutorial1
// Print_String.cpp

#include "pch.h"			// 컴파일 속도 향상
#define _USE_INIT_WINDOW_	// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"		// EasyWin32 사용

// 이 프로그램은 특별한 메시지를 사용하지 않음
NOT_USE_MESSAGE

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "EasyWin32 - Tutorial1";
	// 윈도우 속성 변경 -> 테두리 및 창 속성이 없는 기본적인 속성
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 프로그램 실행하는 메인 함수
int main()
{
	int data = 5;

	// 앞으로 출력될 문자열은 파랑색을 사용하도록 설정
	SetTextColor(RGB(0, 0, 255));

	// data 변수의 10진 정숫값으로 화면에 출력
	printf(100, 50, "data: %d", data);
	printf(100, 70, "결과를 출력했습니다!");

	// 정보를 윈도우에 출력
	ShowDisplay();
	// 프로그램 종료
	return 0;
}