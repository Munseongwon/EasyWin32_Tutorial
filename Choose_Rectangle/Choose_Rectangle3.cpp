// Visual Studio 2022 x86 Release Mode
// 바둑판 모양의 사각형 격자 만들기
// Choose_Rectangle3.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"			// EasyWin32 사용

#define X_COUNT	  12			// X축으로 그려질 사각형의 개수
#define Y_COUNT	  12			// Y축으로 그려질 사각형의 개수
#define GRID_SIZE 30			// 사각형의 폭과 높이에 대한 크기

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "바둑판 모양의 사각형 그리기";
	// 윈도우 속성 변경(제목,창 메뉴,크기 조절x -> 기본)
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 프로그램에서 사용할 내부 데이터
struct AppData
{
	char state[Y_COUNT][X_COUNT]; // 각 사각형의 상태(0:비어있음, 1:검, 2:흰)
	char step; // 검(0), 흰(1)원을 그릴 것인지 저장하는 변수
};

// state 변수에 저장된 상태 값에 따라 사각형과 원을 그림
void DrawRect(AppData *ap_data)
{
	// 어두운 파란 사각형 12 * 12 개 그림(바둑판)
	for (unsigned int y = 0; y < Y_COUNT; ++y) {
		for (unsigned int x = 0; x < X_COUNT; ++x) {
			Rectangle(x * GRID_SIZE, y * GRID_SIZE, (x + 1) * GRID_SIZE + 1, (y + 1) * GRID_SIZE + 1,
				RGB(0, 200, 255), RGB(0, 0, 128));
		}
	}

	// state 값에 따라 흰 or 검은 원을 그리기 위한 색상을 테이블로 만듦
	COLORREF fill_color[2] = {RGB(0,0,0),RGB(255,255,255)};
	for (unsigned int y = 0; y < Y_COUNT; ++y) {
		for (unsigned int x = 0; x < X_COUNT; ++x) {
			if (ap_data->state[y][x]) { // state[y][x] != 0 -> 원이 그려진 상태
				// state[y][x] -> 1(검), 2(흰)
				Ellipse(x * GRID_SIZE, y * GRID_SIZE, (x + 1) * GRID_SIZE + 1, (y + 1) * GRID_SIZE + 1,
					RGB(0,200,255), fill_color[ap_data->state[y][x] - 1]);
			}
		}
	}
	ShowDisplay(); // 정보를 윈도우에 출력
}

// 마우스 왼쪽 버튼이 눌러졌을 때 호출되는 함수 정의
void OnLeftBtnDown(int a_mixed_key, POINT a_pos)
{
	// 폭과 높이 -> GRID_SIZE 크기인 격자 좌표로 변환
	unsigned int x = a_pos.x/GRID_SIZE, y = a_pos.y/GRID_SIZE;
	// 프로그램의 내부 데이터 주소 -> 2차원 배열의 주소
	AppData *p_data = (AppData *)GetAppData();

	// x->0 ~ (X_COUNT-1), y-> 0 ~ (Y_COUNT-1)가 유효한 값
	// state[y][x] -> 0
	if (x < X_COUNT && y < Y_COUNT && !p_data->state[y][x]) {
		// state -> 0, state -> 1, state -> 1, state -> 2
		p_data->state[y][x] = p_data->step + 1;
		// step -> (0=>1, 1=>0)
		p_data->step = !p_data->step;
		// p_data 변수가 가리키는 정보를 가지고 전체 사각형 다시 그린다
		DrawRect(p_data);
	}
}

// 마우스 왼쪽 버튼 누를 때 호출될 함수 등록
MOUSE_MESSAGE(OnLeftBtnDown, NULL, NULL)

// 프로그램 실행하는 메인 함수
int main()
{
	AppData data; // 프로그램이 내부적으로 사용할 메모리 선언
	memset(&data, 0, sizeof(AppData)); // data 변수의 값을 모두 0으로 초기화
	SetAppData(&data, sizeof(data)); // 지정한 변수를 내부 데이터로 저장
	DrawRect(&data); // data 변수에 저장된 정보를 가지고 전체 사각형을 다시 그림
	return 0; // 프로그램 종료(닫힘 버튼 눌린 경우)
}