// Visual Studio 2022 x86 Release Mode
// 버튼 컨트롤 만들고 사용하기
// Create_Use_ButtonCtrl.cpp

#include "pch.h"			// 컴파일 속도 향상
#define _USE_INIT_WINDOW_	// 윈도우 전역 속성 초기화 함수 직접 지정
#include "tipsware.h"		// EasyWin32 사용

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "버튼 컨트롤 예제";
	// 윈도우 속성 변경
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 컨트롤을 조작했을 때 호출할 함수
// 컨트롤 id, 컨트롤 조작 상태, 컨트롤 객체(주소)
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// 버튼은 각각 1000, 1001, 1002번 아이디를 가짐
	if (a_ctrl_id >= 1000 && a_ctrl_id <= 1002) {
		// 각 버튼 클릭 시 사용할 색상을 테이블로 만듦
		COLORREF color[3] = { RGB(100, 200, 255), RGB(200, 100, 255), RGB(200, 200, 255) };
		// 이름이 출력될 위치에 사각형을 그림
		Rectangle()
	}
}

// 프로그램 실행하는 메인 함수
int main()
{
	return 0;
}