#pragma once
#include <atlimage.h>

#define DEFAULT			"default"
#define PURPLE			"purple"

void SetTheme(HWND hwnd, LPCSTR name, INT backgroundColor, CHAR currentExp[], CHAR currentOp[])
{
	CHAR originalDir[MAX_PATH]{};
	CHAR dir[MAX_PATH]{};
	CHAR path[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, originalDir);
	sprintf(dir, "%s%s%s", originalDir, "\\themes\\", name);
	sprintf(path, "%s%s", dir, "\\*");

	WIN32_FIND_DATA data;
	INT i = IDC_BUTTON_0;
	HANDLE hFind = FindFirstFile(path, &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		HBRUSH brush = (HBRUSH)GetStockObject(backgroundColor);
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
		SendMessage(hwnd, WM_ERASEBKGND, (WPARAM)GetDC(hwnd), 0);
		do
		{
			if (data.cFileName[0] == '.' || data.cFileName[0] == (CHAR)"..") continue;
			CImage cimg;
			LPCSTR filename = data.cFileName;
			SetCurrentDirectory(dir);
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