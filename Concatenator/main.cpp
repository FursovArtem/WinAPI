#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR* G_SZ_ARR_PROPERTIES[] =
{
	"очень внимателен на зан€ти€х",
	"хорошо понимает материал",
	"хорошо пишет код",

	"не выполн€ет домашние задани€",
	"не посещает зан€ти€"
};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		SendMessage(GetDlgItem(hwnd, IDC_EDIT_FULLNAME), WM_SETTEXT, 0, (LPARAM)"“упенко ¬асилий »ванович");
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 1024;
			CHAR sz_fullname[MAX_PATH]{};
			CHAR sz_description[SIZE]{};
			HWND hEditFullname = GetDlgItem(hwnd, IDC_EDIT_FULLNAME);
			HWND hEditDescription = GetDlgItem(hwnd, IDC_EDIT_DESCRIPTION);
			HWND boxes[5]{};
			SendMessage(hEditFullname, WM_GETTEXT, MAX_PATH, (LPARAM)sz_fullname);
			strcpy(sz_description, sz_fullname);

			for (int i = 0; i < sizeof(boxes) / sizeof(boxes[0]); i++)
			{
				boxes[i] = GetDlgItem(hwnd, IDC_CHECK_ATTENTION + i);
				if (SendMessage(boxes[i], BM_GETCHECK, 0, 0) == BST_CHECKED) 
					sprintf(sz_description, "%s, %s", sz_description, G_SZ_ARR_PROPERTIES[i]);
			}
			SendMessage(hEditDescription, WM_SETTEXT, 0, (LPARAM)sz_description);
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}