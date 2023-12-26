#pragma once
#include <atlimage.h>
#pragma comment (lib, "gdi32");

#define DEFAULT				"default"
#define PURPLE				"purple"

#define BG_BLACK			bgBlack
#define BG_WHITE			bgWhite
#define BG_LIGHTPURPLE		bgLightPurple

CONST INT BG_BLACK[3] = { 0, 0, 0 };
CONST INT BG_WHITE[3] = { 255, 255, 255 };
CONST INT BG_LIGHTPURPLE[3] = { 0xE4, 0xA0, 0xF7 };
CONST HFONT hfont = CreateFont(32, 14, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Digital-7");

void SetTheme(HWND hwnd, LPCSTR name, CONST INT backgroundColor[3], CHAR currentExp[], CHAR currentOp[])
{
	CHAR originalDir[MAX_PATH]{};
	CHAR imagesDir[MAX_PATH]{};
	CHAR pathfind[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, originalDir);
	sprintf(imagesDir, "%s%s%s", originalDir, "\\themes\\", name);
	sprintf(pathfind, "%s%s", imagesDir, "\\*");

	WIN32_FIND_DATA data;
	INT i = IDC_BUTTON_0;
	HANDLE hFind = FindFirstFile(pathfind, &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		HBRUSH brush = CreateSolidBrush(RGB(backgroundColor[0], backgroundColor[1], backgroundColor[2]));
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
		SendMessage(hwnd, WM_ERASEBKGND, (WPARAM)GetDC(hwnd), 0);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETFONT, (WPARAM)hfont, TRUE);
		do
		{
			if (data.cFileName[0] == '.' || data.cFileName[0] == (CHAR)"..") continue;
			CImage cimg;
			LPCSTR filename = data.cFileName;
			SetCurrentDirectory(imagesDir);
			cimg.Load(filename);
			SendMessage(GetDlgItem(hwnd, i), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)cimg.Detach());
			i++;
		} while (FindNextFile(hFind, &data));
		SetCurrentDirectory(originalDir);
		if (currentExp[0]) SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)currentExp);
		else SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)currentOp);
		FindClose(hFind);
	}
}