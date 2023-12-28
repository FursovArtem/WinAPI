#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "resource.h"
#include "styles.h"
//#include <CommCtrl.h>
//#include <Uxtheme.h>
//#pragma comment (lib, "comctl32")
//#pragma comment (lib, "uxtheme")

#define EXP				g_sz_expr
#define BEXP			g_sz_buffer_expr
#define OP				g_sz_operator
#define LPEXP			(LPARAM)g_sz_expr
#define LPOP			(LPARAM)g_sz_operator

CONST CHAR* g_sz_OPERATORS[] = { "=", "Handler", "+", "-", "*", "/", "<-", "C" };
CONST CHAR g_sz_CLASSNAME[] = "MyCalc";
CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;
CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = 32;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT GetNumber(WPARAM wParam);
CHAR GetOperator(WPARAM wParam);
DOUBLE Calc(DOUBLE a, CHAR operation, DOUBLE b);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, "calc.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "calc.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_CLASSNAME;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	if (RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL, g_sz_CLASSNAME, g_sz_CLASSNAME, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_START_X * 2 + g_i_DISPLAY_WIDTH + 16,
		g_i_START_Y * 2 + g_i_DISPLAY_HEIGHT + g_i_BUTTON_SIZE * 4 + g_i_INTERVAL * 2 + 49,
		NULL, NULL, hInstance, NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
	CONST INT MYSIZE = 32;
	static CHAR g_sz_expr[MYSIZE]{};
	static CHAR g_sz_buffer_expr[MYSIZE]{};
	static CHAR g_sz_operator[1]{};

	static CHAR* g_sz_theme;
	static INT* g_sz_bg;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		/*INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_STANDARD_CLASSES;
		InitCommonControlsEx(&icex);*/

		CreateWindowEx(NULL, "Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY,
			g_i_START_X, g_i_START_Y, g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL);
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				CHAR sz_digit[2] = "";
				sz_digit[0] = i + j + 49;
				CreateWindowEx(NULL, "Button", sz_digit, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i / 3 - 1),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd, (HMENU)(IDC_BUTTON_1 + i + j), GetModuleHandle(NULL), NULL);
			}
		}
		CreateWindowEx(NULL, "Button", "0", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			g_i_BUTTON_START_X, g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)(IDC_BUTTON_0), GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "Button", ".", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)(IDC_BUTTON_POINT), GetModuleHandle(NULL), NULL);
		for (int i = 6; i >= 0; i -= 2)
		{
			for (int j = 0; j < 2; j++)
			{
				CreateWindowEx(NULL, "Button", g_sz_OPERATORS[i + j], WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (4 - i / 2 - 1),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd, (HMENU)(IDC_BUTTON_EQUAL + i + j), GetModuleHandle(NULL), NULL);
			}
		}
		SetTheme(hwnd, THEME_DEFAULT, BLACK, EXP, OP);
		g_sz_theme = (CHAR*)THEME_DEFAULT;
		g_sz_bg = (INT*)BLACK;
		//SetWindowTheme(hwnd, L" ", L" ");
	}
	break;
	case WM_DRAWITEM:
	{
		INT id = wParam;
		DRAWITEMSTRUCT* item = (DRAWITEMSTRUCT*)lParam;
	}
		break;
	/*case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, RGB(0, 0, 100));
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
		SetTextColor(hdc, RGB(255, 0, 0));
		return (LRESULT)hBrush;
	}
	break;*/
	case WM_CONTEXTMENU:
	{
		CHAR* currentTheme = g_sz_theme;
		HMENU hMenu = CreatePopupMenu();
		HMENU hThemes = CreatePopupMenu();
		AppendMenu(hMenu, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hThemes, "Themes");
		AppendMenu(hThemes, MF_BYPOSITION | MF_STRING, CM_DEFAULT, "Default");
		AppendMenu(hThemes, MF_BYPOSITION | MF_STRING, CM_PURPLE, "Purple");
		AppendMenu(hMenu, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_BYPOSITION | MF_STRING, CM_CLOSE, "Close");

		switch (TrackPopupMenuEx(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, LOWORD(lParam), HIWORD(lParam), hwnd, NULL))
		{
		case CM_DEFAULT:
			g_sz_theme = (CHAR*)THEME_DEFAULT;
			g_sz_bg = (INT*)BLACK;
			break;
		case CM_PURPLE:
			g_sz_theme = (CHAR*)THEME_PURPLE;
			g_sz_bg = (INT*)LIGHTPURPLE;
			break;
		case CM_CLOSE: DestroyWindow(hwnd); break;
		}
		if (currentTheme != g_sz_theme) SetTheme(hwnd, g_sz_theme, g_sz_bg, EXP, OP);
	}
	break;
	case WM_KEYDOWN:
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 0x30: case VK_NUMPAD0: case IDC_BUTTON_0:
		case 0x31: case VK_NUMPAD1: case IDC_BUTTON_1:
		case 0x32: case VK_NUMPAD2: case IDC_BUTTON_2:
		case 0x33: case VK_NUMPAD3: case IDC_BUTTON_3:
		case 0x34: case VK_NUMPAD4: case IDC_BUTTON_4:
		case 0x35: case VK_NUMPAD5: case IDC_BUTTON_5:
		case 0x36: case VK_NUMPAD6: case IDC_BUTTON_6:
		case 0x37: case VK_NUMPAD7: case IDC_BUTTON_7:
		case 0x38: case VK_NUMPAD8: case IDC_BUTTON_8:
		case 0x39: case VK_NUMPAD9: case IDC_BUTTON_9:
			sprintf(EXP, "%s%i", EXP, GetNumber(wParam));
			SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
		case VK_ADD: case VK_OEM_PLUS: case IDC_BUTTON_PLUS:
		case VK_SUBTRACT: case VK_OEM_MINUS: case IDC_BUTTON_MINUS:
		case VK_MULTIPLY: case IDC_BUTTON_ASTER:
		case VK_DIVIDE: case IDC_BUTTON_SLASH:
			if (BEXP[0] && OP[0] && EXP[0])										//chain expressions
			{
				double left = atof(BEXP);
				double right = atof(EXP);
				char operation = OP[0];
				CHAR result[MYSIZE]{};
				sprintf(result, "%g", Calc(left, operation, right));
				strcpy(BEXP, result);
				OP[0] = GetOperator(wParam);
				EXP[0] = '\0';
				SendMessage(hEdit, WM_SETTEXT, 0, LPOP); break;
			}
			else if (EXP[0])													//default
			{
				if (EXP[strlen(EXP) - 1] == '.') EXP[strlen(EXP) - 1] = '\0';
				strcpy(BEXP, EXP);
				EXP[0] = '\0';
				OP[0] = GetOperator(wParam);
				SendMessage(hEdit, WM_SETTEXT, 0, LPOP); break;
			}
			else if (OP[0])														//change operator
			{
				OP[0] = GetOperator(wParam);
				SendMessage(hEdit, WM_SETTEXT, 0, LPOP); break;
			}
			break;
		case VK_ESCAPE: case IDC_BUTTON_CLEAR:
			BEXP[0] = '\0';
			OP[0] = '\0';
			EXP[0] = '\0';
			SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
		case VK_BACK: case IDC_BUTTON_BSP:
			if (EXP[0])
			{
				EXP[strlen(EXP) - 1] = '\0';
				SendMessage(hEdit, WM_SETTEXT, 0, LPEXP);
			}
			break;
		case VK_DECIMAL: case IDC_BUTTON_POINT:
			if (!strstr(EXP, "."))
			{
				if (!EXP[0])
				{
					sprintf(EXP, "%i%s", 0, ".");
					SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
				}
				else
				{
					sprintf(EXP, "%s%s", EXP, ".");
					SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
				}
			}
			break;
		case VK_RETURN: case IDC_BUTTON_EQUAL:
			if (BEXP[0] && OP[0] && EXP[0])										//default
			{
				double left = atof(BEXP);
				double right = atof(EXP);
				char operation = OP[0];
				CHAR result[MYSIZE]{};
				sprintf(result, "%g", Calc(left, operation, right));
				strcpy(EXP, result);
				OP[0] = '\0';
				BEXP[0] = '\0';
				SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
			}
			else if (BEXP[0] && OP[0])											//lack of 2nd operand
			{
				OP[0] = '\0';
				strcpy(EXP, BEXP);
				SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
			}
			else SendMessage(hEdit, WM_SETTEXT, 0, LPEXP); break;
			break;
		case IDC_BUTTON_HANDLER:
		{
			if (g_sz_theme == "default")
			{
				SetTheme(hwnd, THEME_PURPLE, LIGHTPURPLE, EXP, OP);
				g_sz_theme = (CHAR*)"purple";
			}
			else
			{
				SetTheme(hwnd, THEME_DEFAULT, BLACK, EXP, OP);
				g_sz_theme = (CHAR*)"default";
			}
		}
		break;
		}
		SetFocus(hwnd);
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
	{
		DeleteObject(hfont);
		DestroyWindow(hwnd);
	}break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
INT GetNumber(WPARAM wParam)
{
	if (wParam == 0x30 || wParam == VK_NUMPAD0 || wParam == IDC_BUTTON_0) return 0;
	else if (wParam == 0x31 || wParam == VK_NUMPAD1 || wParam == IDC_BUTTON_1) return 1;
	else if (wParam == 0x32 || wParam == VK_NUMPAD2 || wParam == IDC_BUTTON_2) return 2;
	else if (wParam == 0x33 || wParam == VK_NUMPAD3 || wParam == IDC_BUTTON_3) return 3;
	else if (wParam == 0x34 || wParam == VK_NUMPAD4 || wParam == IDC_BUTTON_4) return 4;
	else if (wParam == 0x35 || wParam == VK_NUMPAD5 || wParam == IDC_BUTTON_5) return 5;
	else if (wParam == 0x36 || wParam == VK_NUMPAD6 || wParam == IDC_BUTTON_6) return 6;
	else if (wParam == 0x37 || wParam == VK_NUMPAD7 || wParam == IDC_BUTTON_7) return 7;
	else if (wParam == 0x38 || wParam == VK_NUMPAD8 || wParam == IDC_BUTTON_8) return 8;
	else if (wParam == 0x39 || wParam == VK_NUMPAD9 || wParam == IDC_BUTTON_9) return 9;
	else return -1;
}
CHAR GetOperator(WPARAM wParam)
{
	if (wParam == VK_ADD || wParam == VK_OEM_PLUS || wParam == IDC_BUTTON_PLUS) return '+';
	else if (wParam == VK_SUBTRACT || wParam == VK_OEM_MINUS || wParam == IDC_BUTTON_MINUS) return '-';
	else if (wParam == VK_MULTIPLY || wParam == IDC_BUTTON_ASTER) return '*';
	else if (wParam == VK_DIVIDE || wParam == IDC_BUTTON_SLASH) return '/';
	else return NULL;
}
DOUBLE Calc(DOUBLE a, CHAR operation, DOUBLE b)
{
	switch (operation)
	{
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	}
}