// Visual Studio 2022 x86 Release Mode
// 사각형 18개 그리기
// Choose_Rectangle_2.cpp

#include "pch.h"				// 컴파일 속도 향상
#define _USE_INIT_WINDOW_		// 윈도우 전역 속성 초기화 함수
#include "tipsware.h"			// EasyWin32 사용

#define X_COUNT		6			// X축으로 그려질 사각형 개수
#define Y_COUNT		3			// Y축으로 그려질 사각형 개수
#define GRID_SIZE	50			// 사각형의 폭과 높이에 대한 크기

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "사각형 18개 선택하기";
	// 윈도우 속성 변경
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 사각형의 상태가 저장된 2차원 배열 형식의 메모리 주소를 ap_data 포인터로 받음
// 실제로 사용되는 메모리 구조 -> char state[Y_COUNT][X_COUNT]
void DrawRect(char (*ap_data)[X_COUNT])
{
	// 사각형의 채우기 색상 0, 1일 때 달라짐 -> 색상을 테이블로 만듦
	COLORREF fill_color[2] = {RGB(0, 0, 128), RGB(0, 0, 255)};
	// ap_data[y][x] -> 0:어두운 색, 1:밝은 색
	for (unsigned int y = 0; y < Y_COUNT; ++y) {
		for (unsigned int x = 0; x < X_COUNT; ++x) {
			Rectangle(x * GRID_SIZE, y * GRID_SIZE, (x + 1) * GRID_SIZE + 1, (y + 1) * GRID_SIZE + 1, RGB(0, 200, 255), fill_color[ap_data[y][x]]);
		}
	}
	// 정보를 윈도우에 출력
	ShowDisplay();
}

// 마우스 왼쪽 버튼이 눌러졌을 때 호출되는 함수
void OnLeftButtonDown(int a_mixed_key, POINT a_pos)
{
	// 폭과 높이가 GRID_SIZE 크기인 격자 좌표로 변환
	unsigned int x = a_pos.x / GRID_SIZE, y = a_pos.y / GRID_SIZE;
	
	// x->0~X_COUNT-1, y->0~Y_COUNT-1 까지 유효한 값
	if (x < X_COUNT && y < Y_COUNT) {
		// 프로그램의 내부 데이터 주소를 가져옴 -> 2차원 배열의 주소를 받아옴
		char(*p)[X_COUNT] = (char(*)[X_COUNT])GetAppData();
		// 선택한 사각형의 상태 -> (0 => 1) or (1 => 0)
		p[y][x] = !p[y][x]; // (*(p + y))[x] = !(*(p + y))[x];
		// p가 가리키는 메모리 사용 -> 전체 사각형을 다시 그림
		DrawRect(p);
	}
}

// 마우스 왼쪽 버튼을 누를 때 호출될 함수 등록
MOUSE_MESSAGE(OnLeftButtonDown, NULL, NULL)

// 프로그램 실행하는 메인 함수
int main()
{
	// 2차원 배열을 모두 0으로 초기화
	char state[Y_COUNT][X_COUNT] = { {0,}, };
	// 지정한 변수를 내부 데이터로 저장
	SetAppData(state, sizeof(state));
	// state 배열에 저장된 정보를 가지고 전체 사각형을 다시 그림
	DrawRect(state);
	// 프로그램 종료(닫기 -> 강제 종료)
	return 0;
}