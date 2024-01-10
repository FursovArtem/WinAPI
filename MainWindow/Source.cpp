#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <atlimage.h>
#include "resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My first window";

TOOLINFO g_toolItem;
HWND g_hwndTrackingTT;
BOOL g_trackingMouse;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateTrackingTooltip(int toolID, HWND hwnd, WCHAR* pText);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	//cb - Count Bytes
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;

	//wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	//wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIcon = (HICON)LoadImageA(hInstance, "icon1.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImageA(hInstance, "icon2.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hCursor = (HCURSOR)LoadImageA(hInstance, "Background.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0xC8, 0xC8));

	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	wc.lpszMenuName = NULL;
	if (RegisterClassEx(&wc) == NULL)
	{
		MessageBoxA(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна
	CONST INT prim_x = GetSystemMetrics(SM_CXSCREEN);
	CONST INT prim_y = GetSystemMetrics(SM_CYSCREEN);
	CONST INT cx = prim_x / 100 * 75;
	CONST INT cy = prim_y / 100 * 75;
	CONST INT start_x = (prim_x - cx) / 2;
	CONST INT start_y = (prim_y - cy) / 2;
	RECT w = { 0, 0, cx, cy };
	AdjustWindowRect(&w, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindowExA
	(
		NULL,								//ExStyles
		g_sz_WINDOW_CLASS,					//Class name
		g_sz_WINDOW_CLASS,					//Заголовок окна
		WS_OVERLAPPEDWINDOW,				//Такой стиль задается главному окну, он определяет 
		//что у окна будет строка заголовка, кнопки управления
		//окном, масштабируемая граница
		start_x,							//Положение левого верхнего угла окна на экране по X
		start_y,							//Положение левого верхнего угла окна на экране по Y
		w.right - w.left,					//Ширина окна
		w.bottom - w.top,					//Высота окна
		NULL,								//Родительское окно
		NULL,								//Для главного окна этот параметр задает меню, а для
		//дочернего окна - ID-ресурса дочернего окна (IDC_BUTTON, IDC_EDIT и т.д.)
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Запуск цикла сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		IsDialogMessage(hwnd, &msg);
	}
	return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DRAWITEMSTRUCT* dwi;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		InitCommonControls();
		g_hwndTrackingTT = CreateTrackingTooltip(IDC_TOOLTIP, hwnd, NULL);
		CreateWindowEx(NULL, "Button", "Показать координаты мыши", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP,
			20, 20, 250, 35, hwnd, (HMENU)(IDC_CHECKBOX), GetModuleHandle(NULL), NULL);
		HWND hStatus = CreateWindowEx(NULL, STATUSCLASSNAME, "Status bar", WS_CHILD | WS_VISIBLE | WS_BORDER | SBARS_SIZEGRIP,
			0, 0, 0, 0,
			hwnd, (HMENU)(IDM_STATUSBAR), GetModuleHandle(NULL), NULL);
		INT parts[2] = { 64, -1 };
		SendMessage(hStatus, SB_SETPARTS, 2, (LPARAM)parts);

		///////

		CreateWindowEx(NULL, "Button", "Round", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 100, 100, 100, hwnd, (HMENU)IDC_ROUND_BUTTON, GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "Button", "Test", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 300, 300, 100, 100, hwnd, (HMENU)IDC_TEST_BUTTON, GetModuleHandle(NULL), NULL);
	}
	break;
	case WM_DRAWITEM:
	{
		dwi = (DRAWITEMSTRUCT*)lParam;
		CImage cimg;
		switch (dwi->CtlID)
		{
		case IDC_ROUND_BUTTON:
		{
			/*cimg.Load("mask.png");
			HBITMAP mask = cimg.Detach();
			HDC hdcMask = CreateCompatibleDC(dwi->hDC);
			SelectObject(hdcMask, mask);
			BitBlt(dwi->hDC, 0, 0, 100, 100, hdcMask, 0, 0, SRCPAINT);*/
			cimg.Load("test.png");
			HBITMAP bmp = cimg.Detach();
			HDC hdcImg = CreateCompatibleDC(dwi->hDC);
			SelectObject(hdcImg, bmp);
			BitBlt(dwi->hDC, 0, 0, 100, 100, hdcImg, 0, 0, SRCAND);
			//DeleteDC(hdcMask);
			//DeleteObject(mask);
			DeleteDC(hdcImg);
			DeleteObject(bmp);

			SetWindowRgn(GetDlgItem(hwnd, IDC_ROUND_BUTTON), CreateEllipticRgn(0, 0, 101, 101), TRUE);
		}
		break;
		case IDC_TEST_BUTTON:
		{
			POINT pts[] =
			{
				{50, 17}, {51, 17},																	//top-center
				{65, 10}, {78, 10}, {80, 11}, {82, 12}, {84, 13}, {86, 14}, {88, 15}, {90, 16},		//top-right
				{99, 32}, {98, 46}, {97, 49}, {96, 53},	{95, 55},									//right
				{52, 89}, {49, 89},																	//bottom-center
				{5, 55}, {4, 53}, {3, 49}, {2, 46}, {1, 32},										//left
				{10, 16}, {12, 15}, {14, 14}, {16, 13}, {18, 12}, {20, 11}, {25, 10}, {37, 10}		//top-left
			};
			SetWindowRgn(GetDlgItem(hwnd, IDC_TEST_BUTTON), CreatePolygonRgn(pts, 30, WINDING), TRUE);
			cimg.Load("heart.png");
			HBITMAP bmp = cimg.Detach();
			HDC hdcImg = CreateCompatibleDC(dwi->hDC);
			SelectObject(hdcImg, bmp);
			BitBlt(dwi->hDC, 0, 0, 100, 100, hdcImg, 0, 0, SRCAND);
			DeleteDC(hdcImg);
			DeleteObject(bmp);
		}
		break;
		}
	}
	break;
	case WM_MOUSELEAVE:
		SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, FALSE, (LPARAM)&g_toolItem);
		g_trackingMouse = FALSE;
		return FALSE;
	case WM_MOUSEMOVE:
		if (SendMessage(GetDlgItem(hwnd, IDC_CHECKBOX), BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			static INT oldX, oldY;
			INT newX, newY;
			if (!g_trackingMouse)
			{
				TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
				tme.hwndTrack = hwnd;
				tme.dwFlags = TME_LEAVE;
				TrackMouseEvent(&tme);
				SendMessage(g_hwndTrackingTT, TTM_ACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
				g_trackingMouse = TRUE;
			}
			newX = GET_X_LPARAM(lParam);
			newY = GET_Y_LPARAM(lParam);

			if ((newX != oldX) || (newY != oldY))
			{
				oldX = newX;
				oldY = newY;
				CHAR coords[12]{};
				sprintf(coords, "%ix%i", newX, newY);
				g_toolItem.lpszText = (LPSTR)coords;
				SendMessage(GetDlgItem(hwnd, IDM_STATUSBAR), SB_SETTEXT, 1, (LPARAM)coords);
				SendMessage(g_hwndTrackingTT, TTM_SETTOOLINFO, 0, (LPARAM)&g_toolItem);
				POINT pt = { newX, newY };
				ClientToScreen(hwnd, &pt);
				SendMessage(g_hwndTrackingTT, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(newX + 10, newY - 20));
			}
			return FALSE;
		}
		else if (SendMessage(GetDlgItem(hwnd, IDC_CHECKBOX), BM_GETCHECK, 0, 0) == BST_UNCHECKED)
		{
			CHAR text[64]{};
			SendMessage(GetDlgItem(hwnd, IDM_STATUSBAR), WM_GETTEXT, 64, (LPARAM)text);
			if (strcmp(text, "Status bar"))
				SendMessage(GetDlgItem(hwnd, IDM_STATUSBAR), WM_SETTEXT, 0, (LPARAM)"Status bar");
		}
		break;
	case WM_SIZE:
	case WM_MOVING:
	{
		CONST INT SIZE = 256;
		CHAR sz_title[SIZE]{};
		RECT rect;
		GetWindowRect(hwnd, &rect);
		sprintf(sz_title, "%s Position: %ix%i, Size: %ix%i", g_sz_WINDOW_CLASS, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
		SetWindowPos(GetDlgItem(hwnd, IDM_STATUSBAR), NULL, 0, 0, 0, 0, SWP_NOMOVE);
		UpdateWindow(hwnd);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_TEST_BUTTON:
		case IDC_ROUND_BUTTON:
		{
			LONG_PTR lptr = GetWindowLongPtr(GetDlgItem(hwnd, IDC_ROUND_BUTTON), GWL_STYLE);
			MessageBox(hwnd, "Была нажата кнопка", "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDC_CHECKBOX:
			if (IsDlgButtonChecked(hwnd, IDC_CHECKBOX)) CheckDlgButton(hwnd, IDC_CHECKBOX, BST_UNCHECKED);
			else CheckDlgButton(hwnd, IDC_CHECKBOX, BST_CHECKED); break;
		}
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
		if (MessageBox(hwnd, "Вы действительно хотите выйти?", "Выход", MB_YESNO | MB_ICONINFORMATION) == IDYES) DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
HWND CreateTrackingTooltip(int toolID, HWND hwnd, WCHAR* pText)
{
	HWND hwndTT = CreateWindowEx
	(
		WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd, NULL, GetModuleHandle(NULL), NULL
	);

	if (!hwndTT)
	{
		MessageBoxA(hwnd, "Tooltip creation failed", "Error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	g_toolItem.cbSize = sizeof(TOOLINFO);
	g_toolItem.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_ABSOLUTE;
	g_toolItem.hwnd = hwnd;
	g_toolItem.hinst = GetModuleHandle(NULL);
	g_toolItem.lpszText = (LPSTR)pText;
	g_toolItem.uId = (UINT_PTR)hwnd;
	GetClientRect(hwnd, &g_toolItem.rect);

	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&g_toolItem);
	return hwndTT;
}