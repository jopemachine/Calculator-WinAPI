﻿// WinAPI_Calculator.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "resource.h"

#include "TextIndicator.h"
#include "Calculator.h"
#include "Button.h"
#include "KeyMapper.h"

#include <atlbase.h>
#include <queue>
#include <Windows.h>
#include <ShellApi.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINAPICALCULATOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPICALCULATOR));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPICALCULATOR));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPICALCULATOR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOWSIZE_X,
		WINDOWSIZE_Y,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool shiftKeyFlag = false;
	static queue<string> inputQue;
	static Calculator* calculator = Calculator::getInstance();
	static SIZE caretSize;

	switch (message)
	{
		case WM_CREATE: 
		{
			// Caret 만들기
			CreateCaret(hWnd, NULL, 1, 15);
			ShowCaret(hWnd);

			// Row 1
			Button::generate(hWnd, hInst, 25, 110, 70, 50, "(", Button::IDC_BUTTON_LP);
			Button::generate(hWnd, hInst, 100, 110, 70, 50, ")", Button::IDC_BUTTON_RP);
			Button::generate(hWnd, hInst, 175, 110, 70, 50, "<<", Button::IDC_BUTTON_BACK);
			Button::generate(hWnd, hInst, 250, 110, 70, 50, "/", Button::IDC_BUTTON_DIVIDE);

			// Row 2
			Button::generate(hWnd, hInst, 25, 165, 70, 50, "7", Button::IDC_BUTTON_7);
			Button::generate(hWnd, hInst, 100, 165, 70, 50, "8", Button::IDC_BUTTON_8);
			Button::generate(hWnd, hInst, 175, 165, 70, 50, "9", Button::IDC_BUTTON_9);
			Button::generate(hWnd, hInst, 250, 165, 70, 50, "*", Button::IDC_BUTTON_MULTIPLY);

			// Row 3
			Button::generate(hWnd, hInst, 25, 220, 70, 50, "4", Button::IDC_BUTTON_4);
			Button::generate(hWnd, hInst, 100, 220, 70, 50, "5", Button::IDC_BUTTON_5);
			Button::generate(hWnd, hInst, 175, 220, 70, 50, "6", Button::IDC_BUTTON_6);
			Button::generate(hWnd, hInst, 250, 220, 70, 50, "-", Button::IDC_BUTTON_MINUS);

			// Row 4
			Button::generate(hWnd, hInst, 25, 275, 70, 50, "1", Button::IDC_BUTTON_1);
			Button::generate(hWnd, hInst, 100, 275, 70, 50, "2", Button::IDC_BUTTON_2);
			Button::generate(hWnd, hInst, 175, 275, 70, 50, "3", Button::IDC_BUTTON_3);
			Button::generate(hWnd, hInst, 250, 275, 70, 50, "+", Button::IDC_BUTTON_PLUS);

			// Row 5
			Button::generate(hWnd, hInst, 25, 330, 70, 50, "C", Button::IDC_BUTTON_CLEAR);
			Button::generate(hWnd, hInst, 100, 330, 70, 50, "0", Button::IDC_BUTTON_0);
			Button::generate(hWnd, hInst, 175, 330, 70, 50, ".", Button::IDC_BUTTON_DOT);
			Button::generate(hWnd, hInst, 250, 330, 70, 50, "=", Button::IDC_BUTTON_EQU);
			
			// 외부 font import
			if (!AddFontResourceEx(L"../font/Digital-7 Mono.ttf", FR_PRIVATE, NULL)){
				MessageBoxW(hWnd, L"Font loading Error", L"Error", MB_OK);
				exit(0);
			}

		}
		break;

		case WM_GETMINMAXINFO:
		{
			// 창의 크기를 변경하지 못하도록 강제한다
			((MINMAXINFO*)lParam)->ptMaxTrackSize.x = WINDOWSIZE_X;
			((MINMAXINFO*)lParam)->ptMaxTrackSize.y = WINDOWSIZE_Y;
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = WINDOWSIZE_X;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = WINDOWSIZE_Y;
		}
		break;

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{
			case Button::IDC_BUTTON_0:
			case Button::IDC_BUTTON_1:
			case Button::IDC_BUTTON_2:
			case Button::IDC_BUTTON_3:
			case Button::IDC_BUTTON_4:
			case Button::IDC_BUTTON_5:
			case Button::IDC_BUTTON_6:
			case Button::IDC_BUTTON_7:
			case Button::IDC_BUTTON_8:
			case Button::IDC_BUTTON_9:
			case Button::IDC_BUTTON_PLUS:
			case Button::IDC_BUTTON_MINUS:
			case Button::IDC_BUTTON_MULTIPLY:
			case Button::IDC_BUTTON_DIVIDE:
			case Button::IDC_BUTTON_BACK:
			case Button::IDC_BUTTON_LP:
			case Button::IDC_BUTTON_RP:
			case Button::IDC_BUTTON_EQU:
			case Button::IDC_BUTTON_CLEAR:
			case Button::IDC_BUTTON_DOT:
				Button::ButtonEvent ev =
					Button::VkKeyValue((Button::IDC_BUTTON_ID) wmId);
				
				shiftKeyFlag = ev.isShifted;

				KeyMapper::KeyPressed(hWnd, ev.vk_value, shiftKeyFlag, inputQue);
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			TextIndicator::drawOutline(hdc);

			HFONT myFont = CreateFont(19, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Digital-7 Mono");
			HFONT oldFont = (HFONT)SelectObject(hdc, myFont);

			if (TextIndicator::outputFlag) {
				TextIndicator::showResult(hdc);
			}

			while (!inputQue.empty()) {
				TextIndicator::append(inputQue.front());
				inputQue.pop();
			}

			TextIndicator::drawText(hdc);

			SelectObject(hdc, oldFont);
			DeleteObject(myFont);


			string str = TextIndicator::inputExpression();
			const char* chp = str.c_str();

			USES_CONVERSION;

			GetTextExtentPoint(hdc, CA2T(chp), strlen(chp), &caretSize);

			SetCaretPos(307, 45);

			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY: 
		{
			HideCaret(hWnd);
			DestroyCaret();
			PostQuitMessage(0);
			break;
		}

		// 눌린 키에서 손을 땔 때 shiftFlag를 취소한다 
		case WM_KEYUP: 
		{
			shiftKeyFlag = false;
			break;
		}

		case WM_KEYDOWN: 
		{
			// 숫자 키 및 사칙연산 키들의 눌림에 대한 이벤트 처리
			// 들어온 키 입력들을 큐에 넣어놓고 Invalidate로 화면을 무효화해 WM_PAINT를 호출,
			// WM_PAINT에선 큐가 비어 있지 않은 경우 원소들을 꺼내 TextIndicator로 보냄
			KeyMapper::KeyPressed(hWnd, wParam, shiftKeyFlag, inputQue);
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDOK2) 
		{
			ShellExecute(NULL, L"open", L"https://github.com/jopemachine/Calculator-WinAPI", NULL, NULL, SW_SHOWNORMAL);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
