// Visual Studio 2022 x86 Release Mode
// 블로그 글의 html 문서 읽어오기
// Blog_Html_Parsor.cpp 

#include "pch.h"
#define _USE_INIT_WINDOW_
#include "tipsware.h"

// HTTP 프로토콜을 사용하는 함수들이 정의된 wininet.h 헤더 파일 추가
#include <wininet.h>
// winnet.h 헤더에 정의된 함수들 사용 -> wininet.lib 라이브러리 추가
#pragma comment(lib, "wininet.lib")

void InitWindow()
{
	gp_window_title = "블로그 글 html 문서 파싱";
}

DWORD ReadHtmlText(HINTERNET ah_http_file, char* ap_html_string)
{
	// 잠시 지연 효과 -> 메인 스레드에게만 적용, 이벤트 객체 생성
	HANDLE h_wait_event = CreateEvent(NULL, TRUE, 0, NULL);
	// 이벤트 객체 생성에 실패한 경우
	if (NULL == h_wait_event) return 0;
	// html 코드를 저장할 배열 생성
	char buffer[1025];
	// 코드를 읽을 바이트 수, 에러 수, 전체 바이트 수 지정
	DWORD read_bytes = 0, error_count = 0, total_bytes = 0;
	// 1024 바이트 단위로 HTML 코드를 가져옴
	while (InternetReadFile(ah_http_file, buffer, 1024, &read_bytes)) {
		// 1024 단위로 가져온 html 소스를 ap_html_string에 계속 추가
		memcpy(ap_html_string + total_bytes, buffer, read_bytes);
		// 읽은 전체 바이트 수 갱신
		total_bytes += read_bytes;
		if (read_bytes < 1024) {
			// 1024바이트씩 요청 -> html 소스가 1024보다 작음
			// 네트워크 지연 -> 더 작은 크기가 전송
			++error_count;
			if (error_count > 10) break;
			else WaitForSingleObject(h_wait_event, 50); // 50ms 지연하도록 구성
		}
		else error_count = 0;
	}
	// 문자열 끝에 NULL 문자 추가
	*(ap_html_string + total_bytes) = 0;
	// 지연 효과를 위해 생성한 이벤트 객체 제거
	CloseHandle(h_wait_event);
	// 읽어들인 전체 바이트 수 반환
	return total_bytes;
}

// 웹 페이지를 구성하는 HTML 소스를 가져오는 함수
void LoadDataFromWebPage()
{
	// url 주소를 복사할 배열
	char web_url[256];
	// 1001번 에디트 컨트롤에 입력된 웹 페이지 주소를 web_url 배열에 복사
	GetCtrlName(FindControl(1001), web_url, 256);
	// 인터넷을 사용할 세션을 구성
	HINTERNET h_session = InternetOpen("BlogScanner", PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
	// 어떤 사이트에 접속할 지 구성(네이버 블로그 사용)
	HINTERNET h_connect = InternetConnect(h_session, "blog.naver.com", INTERNET_INVALID_PORT_NUMBER, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	// 네이버 블로그에서 원하는 웹 페이지 접속(web_url에 저장된 페이지 엶)
	HINTERNET h_http_file = HttpOpenRequest(h_connect, "GET", web_url, HTTP_VERSION, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

	// 2M Bytes 메모리 할당
	char* p_utf8_html_str = (char*)malloc(2 * 1024 * 1024);
	// 메모리 할당 성공
	if (NULL != p_utf8_html_str) {
		// 웹 페이지 소스를 요청
		if (TRUE == HttpSendRequest(h_http_file, NULL, 0, 0, 0)) {
			// 웹 페이지 전체 소스를 할당된 메모리에 복사
			ReadHtmlText(h_http_file, p_utf8_html_str);

			char* p_ascii_str = NULL;
			// 가져온 웹 페이지 소스 -> utf8 형식의 문자열, ascii 문자열로 변환
			Utf8ToAscii(&p_ascii_str, p_utf8_html_str);
			// 변환에 성공했다면
			if (NULL != p_ascii_str) {
				// 변환된 html 소스를 1000번 에디트 컨트롤에 표시
				SetCtrlName(FindControl(1000), p_ascii_str);
				// 변환에 사용한 메모리를 해제
				free(p_ascii_str);
			}
		}
		// 웹 페이지 소스를 저장하기 위해 할당했던 메모리 해제
		free(p_utf8_html_str);
	}
	// 웹 페이지를 사용하기 위해 할당했던 모든 핸들 해제
	if (NULL != h_http_file) InternetCloseHandle(h_http_file);
	if (NULL != h_connect) InternetCloseHandle(h_connect);
	if (NULL != h_session) InternetCloseHandle(h_session);
}

// 컨트롤을 조작했을 때 호출할 함수
// 매개 변수 -> 컨트롤 아이디, 컨트롤 조작 상태, 선택한 컨트롤 객체
void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void* ap_ctrl)
{
	// 1010번 아이디를 가진 웹 페이지 소스 가져오기 버튼을 누를 경우
	if (a_ctrl_id == 1010) LoadDataFromWebPage();
}

// 컨트롤을 조작했을 때 호출할 함수 등록
CMD_MESSAGE(OnCommand)

int main()
{
	ChangeWorkSize(600, 500); // 작업 영역(너비, 높이)지정
	Clear(0, RGB(82, 97, 124)); // 작업 영역 배경색 지정

	// 웹 페이지 소스를 보여줄 에디트 컨트롤 생성
	// (여러 줄 사용 모드, 가로/세로 자동 스크롤, 수직 스크롤바 사용, 엔터키 사용 모드)
	CreateEdit(5, 5, 590, 458, 1000, ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | WS_HSCROLL);

	// 웹 페이지 주소를 입력할 에디트 컨트롤 생성
	void* p_edit = CreateEdit(210, 470, 385, 24, 1001, 0);

	// 소스를 가져올 샘플 웹페이지 URL을 에디트 컨트롤에 표시
	// blog URL -> https://blog.naver.com/tipsware/223390085274
	// PostView.nhn?blogId=tipsware&logNo=223390085274
	SetCtrlName(p_edit, "PostView.nhn?blogId=tipsware&logNo=223390085274");

	// 버튼 생성
	CreateButton("웹 페이지 소스 가져오기", 5, 468, 200, 28, 1010);
	// 정보를 윈도우에 출력
	ShowDisplay();
	// 닫음(종료 버튼)누를 시 프로그램 강제 종료
	return 0;
}