#include "pch.h"
#include <stdio.h>
#define _USE_INIT_WINDOW_
#include "tipsware.h"

enum Ctrl_Color_ID
{
	LB_TITLE_COLOR	= RGB(200, 255, 200),
	EDIT_TITLE_COLOR = RGB(255, 255, 255),
	SELECT_COLOR	= RGB(200, 255, 255),
	NONSELECT_COLOR = RGB(62, 77, 104)
};

enum TextColor 
{
	NAME_COLOR = RGB(255, 255, 255),
	LEVEL_COLOR = RGB(255, 255, 0),
	IP_COLOR	= RGB(200, 212, 225)
};

enum ListBox_Ctrl_Id
{
	IDC_LB_CHAT = 1000,
	IDC_LB_USER
};

enum Button_Ctrl_Id
{
	ID_BTN_ADD	= 1011,
	ID_BTN_MODIFY,		
	ID_BTN_DEL
};

enum Edit_Ctrl_Id
{
	IDC_EDIT_ID	= 1020,
	IDC_EDIT_PASSWORD,
	IDC_EDIT_NICKNAME
};

enum ComBox_Ctrl_Id
{
	IDC_CB_LEVEL = 1030
};

enum Window_Client_Element
{
	WIN_WIDTH  = 620,
	WIN_HEIGHT = 500,
};

struct UserData
{
	unsigned int h_socket;
	char ip_address[16];
};

typedef struct RegisteredUserData
{
	char user_id[32];
	char user_pw[32];
	char nickname[32];
	char user_level;
	UserData *p_socket;
}RUD;

typedef struct AppData
{
	void *p_user_list;
	void *p_chat_list;
	void *p_server;
}AD;

const char *gp_user_level_str[5] = {"손님", "일반", "고급", "스텝", "관리자"};

void InitWindow()
{
	gp_window_title = "윈도우 채팅 서버";
}

// 새로운 클라이언트 접속할 시 호출되는 함수
// ap_user_data -> 접속한 클라이언트의 정보
// ap_server -> 서버 객체에 대한 주소
void OnNewUser(UserData *ap_user_data, void *ap_server, int a_server_index)
{
	char temp_str[64];
	sprintf_s(temp_str, 64, "새로운 사용자가 %s에서 접속을 했습니다!", ap_user_data->ip_address);
	ListBox_InsertString(FindControl(IDC_LB_CHAT), -1, temp_str);
}

// 클라이언트가 데이터 전송 시 호출되는 함수
int OnClientMessage(CurrentServerNetworkData *ap_data, void *ap_server, int a_server_index)
{
	// 전달된 사용자 정보 -> 자신이 선언한 구조체로 형 변환해서 사용
	// 서버 생성 -> sizeof(UserData) 크기로 만들어 달라고 지정
	// 사용자 정보 -> 내부적으로 UserData 형식으로 관리되고 있음
	UserData *p_user_data = (UserData *)ap_data->mp_net_user;
	char temp_str[128];
	if (ap_data->m_net_msg_id == 1) { // 채팅 데이터 전달(1번은 사용자가 임의적으로 정한 것)
		// 누가 전송했는지 확인 -> 채팅을 전송한 클라이언트의 인터넷 주소를 채팅 데이터 앞에 붙임
		sprintf_s(temp_str, 128, "%s : %s", p_user_data->ip_address, ap_data->mp_net_body_data);
		// 재구성된 채팅 내용을 리스트 박스에 추가
		ListBox_InsertString(FindControl(IDC_LB_CHAT), -1, temp_str);
		// 접속한 모든 클라이언트에게 채팅 내용 다시 전송
		BroadcastFrameData(ap_server, 1, temp_str, strlen(temp_str) + 1);
	}
	return 1;
}

// 클라이언트가 접속을 해제할 시 호출되는 함수
void OnCloseUser(UserData *ap_user_data, void *ap_server, int a_error_flag, int a_server_index)
{
	char temp_str[64];
	if (a_error_flag == 1) {
		sprintf_s(temp_str, 64, "%s에서 접속한 사용자를 강제로 접속 해제했습니다.", ap_user_data->ip_address);
	}
	else {
		sprintf_s(temp_str, 64, "%s에서 사용자가 접속을 해제하였습니다!", ap_user_data->ip_address);
	}
	// 클라이언트의 해제 상태를 리스트 박스에 추가
	ListBox_InsertString(FindControl(IDC_LB_CHAT), -1, temp_str);
}

// ap_user_id에 전달된 ID가 사용자 목록에 있는지 찾는 함수
// 찾았다면 해당 사용자 정보의 주소 반환
// 해당 ID 가진 사용자 X -> null 반환
RUD *FindUserID(AD *ap_data, const char *ap_user_id)
{
	RUD *p_user_info;
	// 리스트 박스에 추가된 항목의 수를 얻음
	int count = ListBox_GetCount(ap_data->p_user_list);
	for (int i = 0; i < count; ++i) {
		// i번째 항목에 저장된 사용자 정보의 주소를 가져옴
		p_user_info = (RUD *)ListBox_GetItemDataPtr(ap_data->p_user_list, i);
		// id가 일치하면 현재 사용자의 주소를 반환
		if (!strcmp(p_user_info->user_id, ap_user_id)) return p_user_info;
	}
	// 지정한 ID를 찾기 실패한 경우
	return NULL;
}

// Edit 컨트롤에 입력된 문자열을 사용자가 지정한 메모리에 복사하는 함수
// 그리고 복사 후에 Edit 컨트롤에 입력된 문자열을 지우는 작업도 함께 진행
void CopyControlDataToMermoy(int a_ctrl_id, char *ap_memory, int a_mem_size)
{
	void *p_edit = FindControl(a_ctrl_id);
	GetCtrlName(p_edit, ap_memory, a_mem_size);
	SetCtrlName(p_edit, "");
}

// 사용자 정보가 입력된 컨트롤에서 정보를 읽어 사용자 목록에 추가하는 함수
void RegisteringUserData()
{
	AD *p_data = (AD *)GetAppData();

	char str[32];
	void *p_edit = FindControl(IDC_EDIT_ID);
	GetCtrlName(p_edit, str, 32);
	int id_len = strlen(str) + 1;
	if (id_len > 5) {
		if (NULL == FindUserID(p_data, str)) {
			SetCtrlName(p_edit, "");
		
			// 사용자 정보를 저장할 메모리 할당
			RUD *p_temp_user = (RUD*)malloc(sizeof(RUD));
			if (NULL != p_temp_user) {
				memcpy(p_temp_user->user_id, str, id_len);
				CopyControlDataToMermoy(IDC_EDIT_PASSWORD, p_temp_user->user_pw, 32);
				CopyControlDataToMermoy(IDC_EDIT_NICKNAME, p_temp_user->nickname, 32);
				p_temp_user->user_level = ComboBox_GetCurSel(FindControl(IDC_CB_LEVEL));
				p_temp_user->p_socket = NULL;

				int index = ListBox_AddString(p_data->p_user_list, p_temp_user->user_id, 0);
				ListBox_SetItemDataPtr(p_data->p_user_list, index, p_temp_user);
				ListBox_SetCurSel(p_data->p_user_list, index);
			}
		}
		else {
			ListBox_InsertString(p_data->p_chat_list, -1,
				"[등록 오류]: 동일한 ID를 가진 사용자가 이미 있습니다!");
		}
	}
	else {
		ListBox_InsertString(p_data->p_chat_list, -1,
			"[등록 오류]: ID는 최소 5자 이상 입력해야 합니다!");
	}
}

// 컨트롤을 조작했을 때 호출할 함수
// 컨트롤 아이디, 컨트롤 조작 상태, 컨트롤 객체(주소)
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl)
{
	// 추가 버튼을 클릭한 경우
	if (ID_BTN_ADD == a_ctrl_id) RegisteringUserData();
	// 삭제 버튼을 클릭한 경우
	else if (ID_BTN_DEL == a_ctrl_id) {
		ListBox_DeleteString(FindControl(IDC_LB_USER), 2);
	}
}

// 사용자 목록에 등록된 정보를 파일에 저장하는 함수
void SaveUserData(const char *ap_file_name)
{
	// 파일을 열어서 사용할 파일 포인터
	FILE *p_file = NULL;
	// fopen_s 함수를 사용하여 파일을 바이너리 형식의 쓰기 모드로 연다
	if (0 == fopen_s(&p_file, ap_file_name, "wb") && p_file != NULL) {
		void *p_ctrl = FindControl(IDC_LB_USER);
		int count = ListBox_GetCount(p_ctrl);
		fwrite(&count, sizeof(int), 1, p_file);
		for (int i = 0; i < count; ++i) {
			fwrite(ListBox_GetItemDataPtr(p_ctrl, i), sizeof(RUD), 1, p_file);
		}
		fclose(p_file);
	}
}

// 프로그램이 종료될 때 사용자가 추가로 할당해서 사용하던 메모리를
// 해제할 수 있도록 자동으로 호출되어지는 함수
void DestroyUserData()
{
	// 사용자 목록에 등록된 정보를 'user_list.dat' 파일에 저장
	SaveUserData("user_list.dat");
	// 사용자 목록이 저장된 리스트 박스의 주소를 얻음
	void *p_data, *p_ctrl = FindControl(IDC_LB_USER);
	// 리스트 박스에 추가된 항목의 수를 얻음
	int count = ListBox_GetCount(p_ctrl);
	// 리스트 박스의 각 항목에 추가된 메모리 모두 제거
	for (int i = 0; i < count; ++i) {
		// i번째 항목에 저장된 주소를 가져옴
		p_data = ListBox_GetItemDataPtr(p_ctrl, i);
		// p_data가 가리키는 메모리 해제
		free(p_data);
	}
	// 리스트 박스의 모든 항목 제거
	ListBox_ResetContent(p_ctrl);
}

// 컨트롤 조작함수 등록
CMD_USER_MESSAGE(OnCommand, NULL, NULL)

// 등록된 사용자 목록에 추가된 항목을 출력하는 함수
// 이 함수는 ListBox에 추가된 항목 수만큼 호출
void DrawUserDataItem(int index, char *ap_str, int a_str_len, void *ap_data, int a_is_selected, RECT *ap_rect)
{
	// 리스트 박스의 각 항목이 선택되었을 때와 아닐 때의 항목 테두리 색상 다르게 처리
	if (a_is_selected) SelectPenObject(SELECT_COLOR);
	else SelectPenObject(NONSELECT_COLOR);
	// 각 항목의 배경을 색상 지정
	SelectBrushObject(NONSELECT_COLOR);
	// 각 항목의 배경으로 사용할 사각형 그림
	Rectangle(ap_rect->left, ap_rect->top, ap_rect->right, ap_rect->bottom);
	// 현재 항목에 연결된 사용자 정보를 사용하기 위해 형변환
	RUD *p_data = (RUD *)ap_data;
	// 글꼴을 '굴림', 크기 12로 설정
	SelectFontObject("굴림", 12);
	// 사용자 아이디와 이름 함께 출력
	TextOut(ap_rect->left + 5, ap_rect->top + 3, NAME_COLOR, "%s(%s)", p_data->user_id, p_data->nickname);
	// 사용자 등급 출력
	TextOut(ap_rect->left + 200, ap_rect->top + 3, LEVEL_COLOR, gp_user_level_str[p_data->user_level]);
	// 사용자가 접속 상태라면 ip 함께 표시
	if (p_data->p_socket != NULL) {
		TextOut(ap_rect->left + 300, ap_rect->top + 3, IP_COLOR, p_data->p_socket->ip_address);
	}
}

// 시스템에 등록된 사용자 정보를 파일에서 읽어 리스트 박스에 추가하는 함수
void LoadUserData(AD *ap_data, const char *ap_file_name)
{
	// 파일을 열어서 사용할 파일 포인터
	FILE *p_file = NULL;
	// fopen_s 함수를 사용하여 파일을 바이너리 형식의 읽기 모드로 엶
	if (0 == fopen_s(&p_file, ap_file_name, "rb") && NULL != p_file) {
		int count = 0, index;
		// 파일에 저장된 사용자 수를 읽음
		fread(&count, sizeof(int), 1, p_file);
		RUD *p_user_info;
		for (int i = 0; i < count; ++i) {
			// 사용자 정보를 저장할 메모리 할당
			p_user_info = (RUD*)malloc(sizeof(RUD));
			if (NULL != p_user_info)
			{
				// 파일에서 사용자 한 명의 정보를 읽어 할당된 메모리에 저장
				fread(p_user_info, sizeof(RUD), 1, p_file);
				// 접속된 소켓 정보 유효x -> 초기화
				p_user_info->p_socket = NULL;
				// 사용자 ID로 정렬되도록 ID를 사용해서 추가
				index = ListBox_AddString(ap_data->p_chat_list, p_user_info->user_id, 0);
				// 새로 추가된 항목에 사용자 정보의 주소를 함께 저장
				ListBox_SetItemDataPtr(ap_data->p_user_list, index, p_user_info);
			}
		}
		fclose(p_file);
	}
}

void CreateUI(AD *ap_data)
{
	SelectFontObject("굴림", 12);
	TextOut(15, 10, LB_TITLE_COLOR, "사용자 채팅글 목록");
	ap_data->p_chat_list = CreateListBox(10, 30, 600, 200, IDC_LB_CHAT);

	TextOut(15, 243, LB_TITLE_COLOR, "등록된 사용자 목록");
	ap_data->p_user_list = CreateListBox(10, 263, 600, 160, IDC_LB_USER, DrawUserDataItem);

	CreateButton("추가", 207, 430, 70, 28, ID_BTN_ADD);
	CreateButton("변경", 280, 430, 70, 28, ID_BTN_MODIFY);
	CreateButton("삭제", 353, 430, 70, 28, ID_BTN_DEL);

	TextOut(15, 476, EDIT_TITLE_COLOR, "아이디");
	CreateEdit(65, 470, 100, 24, IDC_EDIT_ID, 0);
	TextOut(180, 476, EDIT_TITLE_COLOR, "암호");
	CreateEdit(218, 470, 100, 24, IDC_EDIT_PASSWORD, 0);
	TextOut(333, 476, EDIT_TITLE_COLOR, "별명");
	CreateEdit(371, 470, 100, 24, IDC_EDIT_NICKNAME, 0);
	
	TextOut(486, 476, EDIT_TITLE_COLOR, "등급");
	void *p = CreateComboBox(524, 470, 76, 186, IDC_CB_LEVEL);
	for (int i = 0; i < 5; ++i) ComboBox_InsertString(p, i, gp_user_level_str[i], 0);
	ComboBox_SetCurSel(p, 0);
}

int main()
{
	ChangeWorkSize(WIN_WIDTH, WIN_HEIGHT);
	Clear(0, RGB(72, 87, 114));
	StartSocketSystem();
	
	AD p_data;

	// UserData 구조체를 사용하는 서버 생성
	// 자신의 상태에 따라 위에서 정의한
	// OnNewUser, OnClientMessage, OnCloseUser 함수 호출하여 작업 진행
	p_data.p_server = CreateServerSocket(sizeof(UserData), OnNewUser, OnClientMessage, OnCloseUser);
	// 프로그램에 필요한 컨트롤 생성
	CreateUI(&p_data);
	// 지정한 변수를 내부 데이터로 저장
	SetAppData(&p_data, sizeof(AD));
	
	// 'user_list.dat' 파일에서 사용자 정보를 읽어 사용자 목록 리스트 박스에 추가
	LoadUserData(&p_data, "user_list.dat");
	// 127.0.0.1 에서 25001 포트로 서버 서비스 시작
	StartListenService(p_data.p_server, "127.0.0.1", 25001);
	// 정보를 윈도우에 출력
	ShowDisplay();
	// 닫힘 버튼 클릭 시 프로그램 종료
	return 0;
}
