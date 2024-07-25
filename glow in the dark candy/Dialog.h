#pragma once

#include <Windows.h>


class Dialog
{
public:

	static bool Start(const int &width, const int &height);
	static unsigned Width();
	static unsigned Height();
	static unsigned Quality();

private:

	static void *DWAlign(void *ptr);
	static void *Dialog::PushStr(void *ptr, wchar_t *str);
	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	enum
	{
		IDRES = 150,
		IDQ = 200,
		IDL1 = 250,
		IDL2 = 300,
	};
};
