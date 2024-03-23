// Visual Studio 2022 x86 Release Mode
// 오목 GUI 형태
// Choose_Rectangle4.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 전역 속성 초기화 함수 직접 구현하도록 지정
#include "tipsware.h"			// EasyWin32 사용

#define X_COUNT	   19			// X축으로 그려질 줄의 개수
#define Y_COUNT	   19			// Y축으로 그려질 줄의 개수
#define GRID_SIZE  20			// 사각형의 폭과 높이에 대한 크기

// 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "";
	// 윈도우 속성 변경
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 프로그램 사용할 내부 데이터
struct AppData
{
	char state[Y_COUNT][X_COUNT]; // 각 교차점의 상태(0:비어있음, 1:검, 2:흰)
	char step; // 검(0), 흰(1)돌을 놓을 것인지 저장하는 변수
};

// 마우스 왼쪽 버튼이 눌러졌을 때 호출되는 함수 정의
void OnLeftButtonDown(int a_mixed_key, POINT a_pos)
{
	// 폭,높이 -> GRID_SIZE 크기인 격자 좌표로 변환
	unsigned int x = a_pos.x/GRID_SIZE, y = a_pos.y/GRID_SIZE;
	// 프로그램의 내부 데이터 주소 -> 2차원 배열의 주소를 받음
	AppData *p_data = (AppData *)GetAppData();


	// x -> 0 ~ (X_COUNT - 1), y -> 0 ~ (Y_COUNT - 1)
	// state[y][x] -> 0
	if (x < X_COUNT && y < Y_COUNT && !p_data->state[y][x]) {
		// step -> 0, state -> 1, 1 -> 2
		// 흰 or 검은 돌이 놓여졌음을 설정
		p_data->state[y][x] = p_data->step + 1;

		// state 값에 따라 흰색 or 검은 원을 그리기 위해 색상을 테이블로 만든다
		COLORREF fill_color[2] = {RGB(0,0,0),RGB(255,255,255)};
	
		// state[y][x] -> 1(검은 돌), 2(흰 돌)
		Ellipse(x*GRID_SIZE, y*GRID_SIZE, (x+1)*GRID_SIZE+1, (y+1)*GRID_SIZE+1,
			RGB(0,0,0), fill_color[p_data->state[y][x]-1]);
	
		// step 값을 토글(0->1, 1->0)
		p_data->step = !p_data->step;
		ShowDisplay(); // 정보를 윈도우에 출력
	}
}

// 마우스 왼쪽 버튼 누를 때 호출될 함수 등록
MOUSE_MESSAGE(OnLeftButtonDown, NULL, NULL)

// 프로그램 실행하는 메인 함수
int main()
{
	AppData data; // 프로그램이 내부적으로 사용할 메모리 선언
	memset(&data, 0, sizeof(AppData)); // data 변수의 값을 모두 0으로 초기화
	SetAppData(&data, sizeof(data)); // 지정한 변수 내부 데이터 저장
	
	// 바둑판을 그림, 18 * 18 = 324개 그림
	for (unsigned int y = 0; y < Y_COUNT - 1; ++y) {
		for (unsigned int x = 0; x < X_COUNT - 1; ++x) {
			Rectangle(GRID_SIZE/2+x*GRID_SIZE, GRID_SIZE/2+y*)
		}
	}
	return 0; // 프로그램 종료
}