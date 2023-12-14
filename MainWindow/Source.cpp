#include <Windows.h>
#include "resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My first window"; 

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	wc.hIcon = (HICON)LoadImage(hInstance, "icon1.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "icon2.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "Background.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	wc.lpszMenuName = NULL;
	if (RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
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
	HWND hwnd = CreateWindowEx
	(
		NULL,								//ExStyles
		g_sz_WINDOW_CLASS,					//Class name
		g_sz_WINDOW_CLASS,					//Заголовок окна
		WS_OVERLAPPEDWINDOW,				//Такой стиль задается главному окну, он определяет 
											//что у окна будет строка заголовка, кнопки управления
											//окном, масштабируемая граница
		start_x,							//Положение окна на экране по X
		start_y,							//Положение окна на экране по Y
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
	}
	return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}