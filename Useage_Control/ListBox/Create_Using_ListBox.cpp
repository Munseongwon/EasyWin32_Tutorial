// Visual Studio 2022 x86 Release Mode
// 리스트 박스 사용하기
// Create_Using_ListBox.cpp

#include "pch.h"
#define _USE_INIT_WINDOW_
#include "tipsware.h"

NOT_USE_MESSAGE

void InitWindow()
{
	gp_window_title = "리스트 박스 사용하기 - Step1";
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

int main()
{
	ChangeWorkSize(500, 200);
	Clear(0, RGB(82, 97, 124));
	
	void *p_list = CreateListBox(10, 10, 480, 178, 1000);
	
	// 리스트 박스에 문자열을 추가한다
	ListBox_AddString(p_list, "홍길동은 취준생입니다!!", 0);
	ListBox_AddString(p_list, "홍길동은 누구인가요?", 0);
	ListBox_InsertString(p_list, -1, "Test~", 0);
	ListBox_InsertString(p_list, -1, "Tipsware~", 1);

	// 정보를 윈도우에 출력
	ShowDisplay();
	return 0;
}