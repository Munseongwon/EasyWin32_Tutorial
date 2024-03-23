// Visual Studio 2022 x86 Release Mode
// 사각형 6개 선택하기
// Choose_Rectangle.cpp

#include "pch.h"			// 컴파일 속도 향상
#define _USE_INIT_WINDOW_	// 윈도우 전역 속성 초기화 함수 직접 구현하도록 지정	
#include "tipsware.h"		// EasyWin32 사용

// 윈도우 전역 속성 초기화 함수
void InitWindow()
{
	// 창 제목 설정
	gp_window_title = "사각형 18개 선택하기";
	// 윈도우 속성 변경(캡션x, 창 속성x)
	g_wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
}

// 마우스 왼쪽 버튼이 눌러졌을 때 호출되는 함수를 정의한다!
void OnLeftBtnDown(int a_mixed_key, POINT a_pos)
{
	// 출력된 사각형: 일정한 크기 가지고 있음
	// 사각형 시작 위치 -> (0,0)으로 계산해야 함
	// 마우스 x좌표(a_pos)에 20, y좌표에 60을 빼야 함
	// 사각형 폭,높이 -> 60, 마우스 x,y좌표에 60으로 나누기 -> 사각형의 인덱스
	// 6개의 사각형 -> (x->0, y->0), (x->1, y->0), (x->2, y->0), ..
	// POINT 구조체 타입이 signed type이기에 음수 좌표도 처리 -> 양수 좌표만 처리하기 위해서 -> 양 좌표에 unsigned type으로 캐스팅
	unsigned int x = (unsigned int)(a_pos.x - 10) / 60, y = (unsigned int)(a_pos.y - 60) / 60;
	
	if (x < 6 && y < 1) {
		// 6개의 사각형 -> x축 기준 위치, 클릭된 마우스 좌표(a_pos.x) 비교
		// 클릭 x -> 어두운 파란색
		// 클릭 o -> 밝은 파란색
		char *p_data = (char *)GetAppData(); // 프로그램 내부 데이터 주소를 가져옴(메인 함수에서 선언한 state배열의 시작 주소를 가리킴)

		// 선택한 사각형 상태 0 -> 1, 1 -> 0으로 변환
		p_data[x] = !p_data[x]; //*(p_data + x) = !*(p_data + x);
		// 사각형의 테두리 색상이 0, 1일때 달라짐 색상을 테이블로 만듦
		COLORREF border_color[2] = { RGB(0, 200, 255), RGB(0, 100, 200) };
		// 사각형의 채우기 색상이 0, 1일때 달라짐 색상을 테이블로 만듦
		COLORREF fill_color[2] = { RGB(153, 0, 153), RGB(51, 51, 153) };
		// p_data[i] == 0 -> 어두운 색상의 테두리 및 색상으로 사각형 그림
		// p_data[i] == 1 -> 밝은 색상의 테두리 및 색상으로 사각형 그림
		for (int i = 0; i < 6; ++i) {
			Rectangle(10 + i * 60, 60, 10 + (i + 1) * 60, 120, border_color[p_data[i]], fill_color[p_data[i]]);
		}

		/* 조건문을 사용해서 사각형의 색상을 다르게 표현
		for (int i = 0; i < 6; ++i) {
			 각 사각형의 x좌표 시작점, 끝점 비교
			 사각형이 체크되었는지 확인
			if(p_data[i])Rectangle(10 + i * 60, 60, 10 + (i + 1) * 60, 120, RGB(0, 100, 200), RGB(51, 51, 153));
			else Rectangle(10 + x * 60, 60, 10 + (x + 1) * 60, 120, RGB(0, 200, 255), RGB(153, 0, 153));
		}*/	
	}
	ShowDisplay(); // 정보를 윈도우에 출력
}

// 마우스 왼쪽 버튼을 누를 때 호출될 함수 등록
MOUSE_MESSAGE(OnLeftBtnDown, NULL, NULL)

int main()
{
	// 사각형 6개의 상태를 저장할 배열 선언
	// 0-> 선택 안됨, 1->선택됨
	char state[6] = { 0, };
	// 지정한 변수를 내부 데이터로 저장
	SetAppData(&state, sizeof(state));

	// (10, 10)좌표에 파란색으로 안내 메시지 출력
	TextOut(10, 10, RGB(0, 0, 255), "사각형을 클릭하면 색상이 변경됩니다!");
	// 수평 방향으로 6개의 사각형을 나열
	for (int i = 0; i < 6; ++i) {
		Rectangle(10 + i * 60, 60, 10 + (i + 1) * 60, 120, RGB(0, 100, 200), RGB(51, 51, 153));
	}
	// 정보를 윈도우에 출력
	ShowDisplay();
	return 0;
}