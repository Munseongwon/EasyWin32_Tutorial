// Visual Studio 2022 x86 Release Mode
// 리스트 박스 만들기3 - 채팅창 화면 인터페이스 만들기
// 1) 일반 버튼 컨트롤만 사용
// 2) Enter키 입력
// Create_Using_ListBox3.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"			// EasyWin32 사용

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "리스트 박스 만들기 - Step3";
	// 윈도우 속성 변경(기본창 속성)
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// 1002번 버튼이 눌러진 경우
	if (a_ctrl_id == 1002 || (a_ctrl_id == 1001 && a_notify_code == 1000)) {
		char str[64]; // 선택된 항목의 문자열을 저장할 변수
		void *p_edit = FindControl(1001); // 에디트 컨트롤의 주소를 얻음
		// 에디트(1001번)컨트롤에 입력된 문자열을 str배열에 복사
		GetCtrlName(p_edit, str, 64);
		// 에디트 컨트롤에 쓰여진 문자열을 지움
		SetCtrlName(p_edit, "");
		// str 배열의 내용을 리스트 박스의 마지막 위치에 추가
		ListBox_InsertString(FindControl(1000), -1, str, 1);
	}
}

CMD_MESSAGE(OnCommand)

// 프로그램 실행하는 메인 함수
int main()
{
	// 윈도우 클라이언트 영역 설정
	ChangeWorkSize(480, 200);
	// 윈도우 배경색 설정
	Clear(0, RGB(82, 97, 124));
	// 문자열을 저장할 1000번 아이디를 가진 ListBox 생성
	CreateListBox(10, 10, 460, 150, 1000);
	// 문자열을 입력할 에디트 컨트롤 추가
	void *p_edit = CreateEdit(10, 167, 406, 24, 1001, 0);
	// 에디트 컨트롤에 엔터키 입력 -> 1000번에 해당하는 notify_code 발생
	EnableEnterKey(p_edit);
	// 에디트에 입력된 내용을 리스트 박스에 추가할 버튼 컨트롤 생성
	CreateButton("추가", 420, 164, 50, 28, 1002);
	// 프로그램 강제 종료
	return 0;
}