// Visual Studio 2022 x86 Release Mode
// 리스트 박스 사용하기2
// Create_Using_ListBox2.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"			// EasyWin32 사용

// NOT_USE_MESSAGE

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "리스트 박스 사용하기 - Step2";
	// 윈도우 속성 변경(기본적인 윈도우 창 속성)
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 컨트롤을 조작할 때 호출될 함수
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// 1002, 1003버튼이 눌린 경우
	if (a_ctrl_id == 1002 || a_ctrl_id == 1003) {
		// 원본, 사본으로 사용할 컨트롤 주소로 사용될 변수
		void *p_src, *p_dest;
		// 왼쪽 -> 오른쪽으로 항목 이동
		if (a_ctrl_id == 1002) {
			p_src = FindControl(1000); // 왼쪽이 원본
			p_dest = FindControl(1001); // 오른쪽이 사본
		}
		// 오른쪽 -> 왼쪽으로 항목 이동
		else {
			p_src = FindControl(1001); // 오른쪽이 원본
			p_dest = FindControl(1000); // 왼쪽이 사본
		}
		// 선택한 항목의 위치를 얻음
		int index = ListBox_GetCurSel(p_src);
		// 해당 리스트 박스에서 선택된 항목이 없다면 -1 반환
		if (index != -1) {
			char str[32]; // 선택된 항목의 문자열을 저장할 변수
			ListBox_GetText(p_src, index, str, 32); // 원본 리스트의 index에 있는 문자열을 str 변수에 저장
			ListBox_DeleteString(p_src, index); // 원본 리스트에서 선택한 항목 삭제
			ListBox_AddString(p_dest, str, 1); // 사본 리스트 박스에 읽어온 문자열 추가(커서 설정)
		}
	}
}

// 컨트롤을 조작할 때 호출될 함수 등록
CMD_MESSAGE(OnCommand)

// 프로그램 실행하는 메인 함수
int main()
{
	// 윈도우 클라이언트 영역 설정
	ChangeWorkSize(570, 200);
	// 윈도우 배경색 설정
	Clear(0, RGB(82, 97, 124));
	// 사용할 문자열을 테이블로 만듦
	const char *p_attr[6] = {"나이","이름","주소","직급","소속","전화번호"};
	// 왼쪽에 1000번 아이디를 가진 리스트 박스 생성
	void* p_list = CreateListBox(10, 10, 100, 150, 1000);
	// ListBox에 여섯 개의 문자열을 추가(오름차순 기능 사용)
	for (unsigned char i = 0; i < 6; ++i)ListBox_AddString(p_list, p_attr[i], 0);
	// 오른쪽에 1001번 아이디를 가진 리스트 박스 생성
	CreateListBox(180, 10, 100, 150, 1001);
	// 왼쪽 -> 오른쪽 버튼 컨트롤 생성
	CreateButton("▶", 120, 50, 50, 28, 1002);
	// 오른쪽 -> 왼쪽 버튼 컨트롤 생성
	CreateButton("◀", 120, 80, 50, 28, 1003);
	// 정보를 윈도우에 출력
	ShowDisplay();
	// 종료 버튼을 누른 경우(강제 종료)
	return 0;
}