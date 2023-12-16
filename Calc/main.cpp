#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "resource.h"

CONST CHAR* g_sz_OPERATORS[] = { "+", "-", "*", "/", "<-", "C" };
CONST INT MYSIZE = 32;
CHAR g_sz_expr[MYSIZE]{};
CHAR g_sz_buffer_expr[MYSIZE]{};
CHAR g_operator[1]{};

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
DOUBLE Calc(double a, char operation, double b);

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
		g_i_START_X + g_i_DISPLAY_WIDTH + g_i_INTERVAL * 5,
		g_i_DISPLAY_HEIGHT + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 5,
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
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx(NULL, "Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
			g_i_START_X, g_i_START_Y, g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL);
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				CHAR sz_digit[2] = "";
				sz_digit[0] = i + j + 49;
				CreateWindowEx(NULL, "Button", sz_digit, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i / 3 - 1),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd, (HMENU)(IDC_BUTTON_1 + i + j), GetModuleHandle(NULL), NULL);
			}
		}
		CreateWindowEx(NULL, "Button", "0", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X, g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)(IDC_BUTTON_0), GetModuleHandle(NULL), NULL);
		CreateWindowEx(NULL, "Button", ".", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)(IDC_BUTTON_POINT), GetModuleHandle(NULL), NULL);
		////////////////////////////
		for (int i = 4; i >= 0; i -= 2)
		{
			for (int j = 0; j < 2; j++)
			{
				CreateWindowEx(NULL, "Button", g_sz_OPERATORS[i + j], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i / 2 - 1),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd, (HMENU)(IDC_BUTTON_PLUS + i + j), GetModuleHandle(NULL), NULL);
			}
		}
		CreateWindowEx(NULL, "Button", "=", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)(IDC_BUTTON_EQUAL), GetModuleHandle(NULL), NULL);
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
			sprintf(g_sz_expr, "%s%i", g_sz_expr, GetNumber(wParam));
			SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
		case VK_ADD: case VK_OEM_PLUS: case IDC_BUTTON_PLUS:
		case VK_SUBTRACT: case VK_OEM_MINUS: case IDC_BUTTON_MINUS:
		case VK_MULTIPLY: case IDC_BUTTON_ASTER:
		case VK_DIVIDE: case IDC_BUTTON_SLASH:
			if (g_sz_expr[0])
			{
				if (g_sz_expr[strlen(g_sz_expr) - 1] == '.') g_sz_expr[strlen(g_sz_expr) - 1] = '\0';
				strcpy(g_sz_buffer_expr, g_sz_expr);
				g_sz_expr[0] = '\0';
				g_operator[0] = GetOperator(wParam);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_operator); break;
			}
			else if (g_operator[0])
			{
				g_operator[0] = GetOperator(wParam);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_operator); break;
			}
			break;
		case IDC_BUTTON_CLEAR:
			g_sz_expr[0] = '\0';
			SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
		case VK_BACK: case IDC_BUTTON_BSP:
			if (g_sz_expr[0]) g_sz_expr[strlen(g_sz_expr) - 1] = '\0';
			SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
		case VK_DECIMAL: case IDC_BUTTON_POINT:
			if (!strstr(g_sz_expr, "."))
			{
				if (!g_sz_expr[0])
				{
					sprintf(g_sz_expr, "%i%s", 0, ".");
					SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
				}
				else
				{
					sprintf(g_sz_expr, "%s%s", g_sz_expr, ".");
					SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
				}
			}
			break;
		case VK_RETURN: case IDC_BUTTON_EQUAL:
			if (g_sz_buffer_expr[0] && g_operator[0] && g_sz_expr[0])
			{
				double left = atof(g_sz_buffer_expr);
				double right = atof(g_sz_expr);
				char operation = g_operator[0];
				CHAR result[MYSIZE]{};
				sprintf(result, "%f", Calc(left, operation, right));

				strcpy(g_sz_expr, result);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
			}
			else if (g_sz_buffer_expr[0] && g_operator[0])
			{
				g_operator[0] = '\0';
				strcpy(g_sz_expr, g_sz_buffer_expr);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
			}
			else SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)g_sz_expr); break;
			break;
		}
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
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
DOUBLE Calc(double a, char operation, double b)
{
	switch (operation)
	{
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	}
}