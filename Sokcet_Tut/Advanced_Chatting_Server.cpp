// Visual Studio 2022 x86 Release Mode
// 채팅 서버(심화)
// Advanced_Chatting_Server.cpp

#include "pch.h"					// 컴파일 속도 향상
#include <stdio.h>					// sprintf_s 함수 사용
#define _USE_INIT_WINDOW_			// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"				// EasyWin32 사용

#define ID_LB_CHAT		1000		// 채팅 목록 리스트 박스 아이디
#define ID_LB_USER		1001		// 사용자 목록 리스트 박스 아이디

#define ID_BTN_ADD		1011		// 추가 버튼 아이디
#define ID_BTN_MODIFY	1012		// 변경 버튼 아이디
#define ID_BTN_DEL		1013		// 삭제 버튼 아이디

#define ID_EDIT_ID		1020		// 사용자 아아디 에디트 아이디
#define ID_EDIT_PW		1021		// 사용자 암호 에디트 아이디
#define ID_EDIT_NICK	1022		// 사용자 닉네임 에디트 아이디

#define ID_CB_LEVEL		1030		// 사용자 카페 회원 등급 아이디

const char *gp_user_level_str[5] = { "손님","일반","고급", "스텝", "관리자" }; // 회원 등급 나타내는 문자열

struct UserData;

// 서버에 접속하는 사용자를 관리하기 위한 구조체
typedef struct RegisteredUserData
{
	char user_id[32];		// 사용자 아이디
	char user_pw[32];		// 사용자 암호
	char user_nick[32];		// 사용자 닉네임
	char level;				// 사용자 등급(0-손님, 1-일반, 2-고급, 3-스텝, 4-관리자)
	UserData *p_socket;		// 접속 중이라면 사용중인 소켓 주소
}RUD;

// 프로그램 내에서 사용하는 내부 데이터
struct AppData
{
	void *p_user_list;		// 사용자 목록 리스트 박스 주소
	void *p_chat_list;		// 채팅 목록 리스트 박스 주소
	void *p_server;			// 서버 소켓 주소
};

// 서버에 접속하는 사용자를 관리하기 위한 구조체
struct UserData
{
	unsigned int h_socket;  // 소켓 핸들
	char ip_address[16];	// 사용자 접속 ip주소
};

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	gp_window_title = "윈도우 채팅 서버";
}

// 새로운 클라이언트가 접속할 때 호출하는 함수
// ap_user_data -> 접속한 클라이언트 정보
// ap_server -> 서버 객체의 주소
// a_server_index -> 서버 번호
void OnNewUser(UserData *ap_user_data, void *ap_server, int a_server_index)
{
	// 사용자의 ip주소를 복사할 문자열 배열
	char temp_str[64];
	// 접속한 클라이언트 정보 -> 메모리 내에 복사
	sprintf_s(temp_str, 64, "새로운 사용자가 %s에서 접속했습니다", ap_user_data->ip_address);
	// 접속된 클라이언트 주소가 담긴 문자열을 채팅 목록 리스트 박스에 추가
	ListBox_InsertString(FindControl(ID_LB_CHAT), -1, temp_str);
}

// 클라이언트가 데이터를 전송할 때 호출하는 함수
int OnClientMessage(CurrentServerNetworkData *ap_data, void *ap_server, int a_server_index)
{
	// 전달된 사용자 정보 -> 자신이 선언한 구조체로 형 변환
	// 서버 생성 -> sizeof(UserData) 크기로 지정
	// 사용자 정보 -> UserData 형식으로 관리
	UserData *p_user_data = (UserData *)ap_data->mp_net_user;
	// 사용자 정보를 저장할 문자열 배열
	char temp_str[128];
	// 채팅 데이터가 전송되었을 경우(여기서는 1로 지정)
	if (ap_data->m_net_msg_id == 1) {
		// 누가 전송했는 지 확인
		// 채팅 데이터 + 클라이언트 ip주소 -> 채팅 내용
		sprintf_s(temp_str, 128, "%s : %s", p_user_data->ip_address, ap_data->mp_net_body_data);
		// 재구성된 채팅 내용을 채팅 목록 리스트 박스에 표시
		ListBox_InsertString(FindControl(ID_LB_CHAT), -1, temp_str);
		// 접속한 모든 클라이언트에게 데이터 전송
		BroadcastFrameData(ap_server, 1, temp_str, strlen(temp_str) + 1);
	}
	return 1;
}

// 클라이언트가 접속을 해제할 때 호출하는 함수
void OnCloseUser(UserData *ap_user_data, void *ap_server, int a_error_flag, int a_server_index)
{
	// 클라이언트 접속 여부를 복사하기 위한 문자열
	char temp_str[64];
	// 오류로 인한 클라이언트 강제 해제
	if (a_error_flag == 1) {
		sprintf_s(temp_str, 64, "%s에서 접속한 사용자를 강제 해제했습니다!", ap_user_data->ip_address);
	}
	// 정상적인 해제
	else {
		sprintf_s(temp_str, 64, "%s에서 사용자가 접속 해제했습니다!", ap_user_data->ip_address);
	}
	// 클라이언트의 상태를 채팅 리스트 박스에 표시
	ListBox_InsertString(FindControl(ID_LB_CHAT), -1, temp_str);
}

// ap_user_id -> 전달된 id가 사용자 목록에 있는 지 찾는 함수
// id(o) -> 사용자 정보 주소 반환
// id(x) -> NULL 반환
RUD *FindUserID(AppData *ap_data, const char *ap_user_id)
{
	RUD *p_user_info;  // 사용자 정보 주소 선언
	int count = ListBox_GetCount(ap_data->p_user_list); // 리스트 박스에 추가된 항목의 수를 구함
	for (int i = 0; i < count; ++i) {
		// i번째 항목에 저장된 사용자 정보의 주소를 가져옴
		p_user_info = (RUD *)ListBox_GetItemDataPtr(ap_data->p_user_list, i);
		// id가 일치 -> 현재 사용자의 주소 반환
		if (!strcmp(p_user_info->user_id, ap_user_id)) return p_user_info;
	}
	return NULL; // 지정한 id를 찾기에 실패한 경우
}

// Edit 컨트롤에 입력된 문자열을 사용자가 지정한 메모리에 복사하는 함수
// 메모리 복사 -> 입력된 문자열 삭제
void CopyControlDataToMemory(int a_ctrl_id, char *ap_memory, int a_mem_size)
{
	
}

// 사용자 정보가 입력된 컨트롤에서 정보를 읽어 사용자 목록에 추가하는 함수
void RegisteringUserData()
{
	AppData *p_data = (AppData *)GetAppData(); // 프로그램 내부 데이터 주소 얻음
}

// 컨트롤을 조작했을 때 호출할 함수
// 컨트롤 아이디, 컨트롤 조작 상태, 컨트롤 객체
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// [추가] 버튼을 누른 경우
	if (a_ctrl_id == ID_BTN_ADD) RegisteringUserData();
}

// 사용자 목록에 등록된 정보를 파일에 저장하는 함수
void SaveUserData(const char *ap_file_name)
{

}

// 프로그램이 종료될 때 사용자가 추가로 할당한 메모리를
// 해제할 수 있도록 자동으로 호출되는 함수
void DestroyUserData()
{

}

// 컨트롤이 조작되면 -> OnCommand, 프로그램 종료 -> DestroyUserData
// CMD_USER_MESSAGE(OnCommand, DestroyUserData, NULL)


// 등록된 사용자 목록에 추가된 항목을 추가하는 함수
// ListBox에 추가된 항목 수 만큼 출력
void DrawUserDataItem(int index, char *ap_str, int a_str_len, void *ap_data, int a_is_selected, RECT *ap_rect)
{

}

// 시스템에 등록된 사용자 정보를 파일에서 읽어 리스트 박스에 추가하는 함수
void LoadUserData(AppData *ap_data, const char *ap_file_name)
{

}

// 화면 내의 컨트롤을 그리는 함수
void CreateUI(AppData* ap_data)
{
	SelectFontObject("굴림", 12);
	TextOut(15, 10, RGB(200, 255, 200), "사용자 채팅글 목록");
	ap_data->p_chat_list = CreateListBox(10, 30, 600, 200, ID_LB_CHAT);
	TextOut(15, 243, RGB(200, 255, 200), "등록된 사용자 목록");
	ap_data->p_user_list = CreateListBox(10, 263, 600, 160, ID_LB_USER);

	CreateButton("추가", 207, 430, 70, 28, ID_BTN_ADD);
	CreateButton("변경", 280, 430, 70, 28, ID_BTN_MODIFY);
	CreateButton("삭제", 353, 430, 70, 28, ID_BTN_DEL);

	TextOut(15, 476, RGB(255, 255, 255), "아이디: ");
	CreateListBox(65, 470, 100, 24, ID_EDIT_ID, 0);
	TextOut(180, 476, RGB(255, 255, 255), ": ");
	CreateListBox(218, 470, 100, 24, ID_EDIT_PW, 0);
	TextOut(333, 476, RGB(255, 255, 255), "아이디: ");
	CreateListBox(371, 470, 100, 24, ID_EDIT_NICK, 0);
	TextOut(486, 476, RGB(255, 255, 255), "등급: ");
	void *p_combo = CreateComboBox(524, 470, 76, 186, ID_CB_LEVEL);
	for (unsigned char i = 0; i < 5; ++i)ComboBox_InsertString(p_combo, i, gp_user_level_str[i], 0);
	ComboBox_SetCurSel(p_combo, 0);
}

NOT_USE_MESSAGE

// 프로그램 실행하는 메인 함수
int main()
{
	ChangeWorkSize(600, 500);	// 윈도우 작업 영역 설정(600 * 500)
	Clear(0, RGB(72, 87, 114));	// 윈도우 배경색 설정

	AppData data;	// 프로그램 내의 내부 메모리 설정
	SetAppData(&data, sizeof(AppData));	// 지정한 변수를 내부 데이터로 저장

	CreateUI(&data);


	ShowDisplay();
	return 0;
}