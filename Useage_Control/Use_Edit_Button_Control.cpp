// Visual Studio 2022 x86 Release Mode
// 에디트 컨트롤과 버튼 사용하기
// Use_Edit_Button_Control.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"			// EasyWin32 사용

// 해당 프로그램은 윈도우 메시지 사용 안함
// NOT_USE_MESSAGE

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "에디트 컨트롤과 버튼 사용하기";
	// 기본 테두리를 가진 윈도우 속성
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 컨트롤을 조작했을 때 발생하는 함수
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// 1001번 아이디를 가진 버튼 컨트롤 실행
	if (a_ctrl_id == 1001) {
		char str[32]; // 컨트롤에 입력된 문자열을 저장하는 변수
		// 1000번 아이디를 가진 컨트롤(에디트 컨트롤)에서 문자열을 읽음
		GetCtrlName(FindControl(1000), str, 32);
		// 문자열이 출력될 위치에 사각형을 그림
		Rectangle(10, 70, 400, 130, RGB(0, 0, 200), RGB(0, 0, 100));
		// 굴림, 26, 강조 형식의 글꼴 설정
		SelectFontObject("굴림", 26, 1);
		// 에디트 컨트롤에서 읽은 문자열 출력
		TextOut(30, 85, RGB(0, 200, 255), str);
		// 정보를 윈도우에 출력
		ShowDisplay();
	}
}

// 컨트롤을 조작했을 때 호출할 함수 등록
CMD_MESSAGE(OnCommand)

// 프로그램 실행하는 메인 함수
int main()
{
	// 클라이언트 영역 설정
	ChangeWorkSize(500, 200);
	// 윈도우 배경색을 입힘
	Clear(0, RGB(82, 97, 124));
	
	// 1000번 아이디를 가진 에디트 컨트롤 생성
	CreateEdit(10, 10, 200, 20, 1000, 0);
	// 1001번 아이디를 가진 버튼 컨트롤 생성
	CreateButton("적용", 10, 36, 98, 28, 1001);
	// 출력된 정보를 윈도우에 보여줌
	// ShowDisplay();
	// 닫힘 버튼을 누른 경우(강제 종료)
	return 0;
}