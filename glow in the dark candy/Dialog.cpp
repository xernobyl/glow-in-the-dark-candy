#include "Dialog.h"
#include <stdio.h>

#include <dwmapi.h>


static int width, height, quality;
static double aspectratio;

static unsigned *heigths;
static unsigned nheights = 0;


unsigned Dialog::Width()
{
	return width;
}


unsigned Dialog::Height()
{
	return height;
}


unsigned Dialog::Quality()
{
	return quality;
}


bool Dialog::Start(const int &w, const int &h)
{
	width = w;
	height = h;
	aspectratio = (double)w/h;
	quality = 4;

	HGLOBAL hgbl;
	LPDLGTEMPLATE lpdt;
	LPDLGITEMTEMPLATE lpdit;
	LPWORD lpw;
	LRESULT ret;

	hgbl = GlobalAlloc(GMEM_ZEROINIT, 4096);
	lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
 
	// Define a dialog box
	lpdt->style = WS_POPUP | DS_CENTER | DS_SETFONT | WS_BORDER;
	lpdt->dwExtendedStyle = WS_EX_CONTROLPARENT | WS_EX_TOPMOST | WS_EX_COMPOSITED | WS_EX_APPWINDOW;
	lpdt->cdit = 6;			// controls
	lpdt->cx = 135;
	lpdt->cy = 65;

	lpw = (LPWORD)(lpdt + 1);
	*lpw++ = 0;				// menu
	*lpw++ = 0;				// class

	lpw = (LPWORD)PushStr(lpw, L"HELLO");
	*lpw++ = 8;				// font size
	lpw = (LPWORD)PushStr(lpw, L"Futura Md");

	// IDOK
	lpdit = (LPDLGITEMTEMPLATE)DWAlign(lpw);
	lpdit->x  = 70;
	lpdit->y  = 45;
	lpdit->cx = 60;
	lpdit->cy = 15;
	lpdit->id = IDOK;
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT | WS_TABSTOP;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;		// Button class

	lpw = (LPWORD)PushStr(lpw, L"leet");
	*lpw++ = 0;				// No creation data

	// IDCANCEL

	lpdit = (LPDLGITEMTEMPLATE)DWAlign(lpw);
	lpdit->x  = 5;
	lpdit->y  = 45;
	lpdit->cx = 60;
	lpdit->cy = 15;
	lpdit->id = IDCANCEL;
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT | WS_TABSTOP;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;		// Button class atom

	lpw = (LPWORD)PushStr(lpw, L"quit");

	*lpw++ = 0;				// No creation data

	// IDRES

	lpdit = (LPDLGITEMTEMPLATE)DWAlign(lpw);
	lpdit->x  = 5;
	lpdit->y  = 5;
	lpdit->cx = 60;
	lpdit->cy = 15;
	lpdit->id = IDRES;		// Text identifier
	lpdit->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_HORZ;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0084;		// scroll bar class

	*lpw++ = 0;
	*lpw++ = 0;

	// IDQ

	lpdit = (LPDLGITEMTEMPLATE)DWAlign(lpw);
	lpdit->x  = 5;
	lpdit->y  = 25;
	lpdit->cx = 60;
	lpdit->cy = 15;
	lpdit->id = IDQ;
	lpdit->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_HORZ;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0084;		// scroll bar class

	*lpw++ = 0;
	*lpw++ = 0;

	// IDL1

	lpdit = (LPDLGITEMTEMPLATE)DWAlign(lpw);
	lpdit->x  = 70;
	lpdit->y  = 5;
	lpdit->cx = 60;
	lpdit->cy = 15;
	lpdit->id = IDL1;
	lpdit->style = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0082;		// static class

	wchar_t tmp[16];
	wsprintfW(tmp, L"%u x %u", width, height);
	lpw = (LPWORD)PushStr(lpw, tmp);
	*lpw++ = 0;				// No creation data

	// IDL2

	lpdit = (LPDLGITEMTEMPLATE)DWAlign(lpw);
	lpdit->x  = 70;
	lpdit->y  = 25;
	lpdit->cx = 60;
	lpdit->cy = 15;
	lpdit->id = IDL2;
	lpdit->style = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0082;		// static class

	lpw = (LPWORD)PushStr(lpw, L"very high");
	*lpw++ = 0;				// No creation data

	heigths = new unsigned[height];
	nheights = 0;

	for(int i = height/8; i <= height; i+=2)
	{
		double tf = (double)width * i / height;
		int ti = width * i / height;

		if(tf == (double) ti)
			heigths[nheights++] = i;
	}
	
	GlobalUnlock(hgbl);
	ret = DialogBoxIndirect(0, (LPDLGTEMPLATE)hgbl, 0, (DLGPROC)DlgProc);
	GlobalFree(hgbl);

	delete [] heigths;

	return ret != 0;
}


void *Dialog::DWAlign(void *ptr)
{
	return (void*)(((DWORD)ptr+3)&~3);
}


void *Dialog::PushStr(void *ptr, wchar_t *str)
{
		wcscpy((wchar_t*)ptr, str);
		return (wchar_t*)ptr+(wcslen(str)+1);
}


INT_PTR CALLBACK Dialog::DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//static DWM_BLURBEHIND blur;
	//static MARGINS margins = {-1};
	
	static HWND hres, hq, hidl1, hidl2;
	static SCROLLINFO lpsi;

	switch(msg)
	{
		case WM_INITDIALOG:
			{
				//SetLayeredWindowAttributes(hDlg, 0, 128, LWA_ALPHA);
				
				//blur.fEnable = true;
				//blur.dwFlags = DWM_BB_ENABLE;// | DWM_BB_BLURREGION;
				//blur.hRgnBlur = 0;
				
				//DwmEnableBlurBehindWindow(hDlg, &blur);
				//DwmExtendFrameIntoClientArea(hDlg, &margins);
				
				hres = GetDlgItem(hDlg, IDRES);
				hq = GetDlgItem(hDlg, IDQ);
				hidl1 = GetDlgItem(hDlg, IDL1);
				hidl2 = GetDlgItem(hDlg, IDL2);

				lpsi.cbSize = sizeof(SCROLLINFO);
				lpsi.fMask = SIF_POS | SIF_RANGE | SIF_TRACKPOS;
				lpsi.nMin = 0;
				lpsi.nMax = nheights-1;
				lpsi.nPos = nheights-1;
				lpsi.nTrackPos = nheights-1;

				SetScrollInfo(hres, SB_CTL, &lpsi, true);

				lpsi.nMax = 4;
				lpsi.nPos = quality;
				lpsi.nTrackPos = quality;

				SetScrollInfo(hq, SB_CTL, &lpsi, true);

				break;
			}

		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				case IDOK:
					if(HIWORD(wParam)==BN_CLICKED)
						return EndDialog(hDlg, 1);
					break;

				case IDCANCEL:
					if(HIWORD(wParam)==BN_CLICKED)
						return EndDialog(hDlg, 0);
					break;
			}
			break;

		case WM_HSCROLL:
			if((HWND)lParam == hres)
			{	
				lpsi.fMask = SIF_TRACKPOS;

				GetScrollInfo(hres, SB_CTL, &lpsi);
				lpsi.fMask = SIF_POS;
				lpsi.nPos = lpsi.nTrackPos;
				SetScrollInfo(hres, SB_CTL, &lpsi, true);

				height = heigths[lpsi.nTrackPos];
				width = aspectratio*height;

				wchar_t ts[16];
				wsprintfW(ts, L"%u x %u", width, height);
				SendDlgItemMessage(hDlg, IDL1, WM_SETTEXT, 0, (LPARAM)ts);
			}
			else
			{
				lpsi.fMask = SIF_TRACKPOS;

				GetScrollInfo(hq, SB_CTL, &lpsi);

				lpsi.fMask = SIF_POS;
				lpsi.nPos = lpsi.nTrackPos;

				SetScrollInfo(hq, SB_CTL, &lpsi, true);

				quality = lpsi.nTrackPos;

				switch(quality)
				{
				case 0:
					SendDlgItemMessage(hDlg, IDL2, WM_SETTEXT, 0, (LPARAM)L"very low");
					break;

				case 1:
					SendDlgItemMessage(hDlg, IDL2, WM_SETTEXT, 0, (LPARAM)L"low");
					break;

				case 2:
					SendDlgItemMessage(hDlg, IDL2, WM_SETTEXT, 0, (LPARAM)L"medium");
					break;

				case 3:
					SendDlgItemMessage(hDlg, IDL2, WM_SETTEXT, 0, (LPARAM)L"high");
					break;

				case 4:
					SendDlgItemMessage(hDlg, IDL2, WM_SETTEXT, 0, (LPARAM)L"very high");
					break;
				}
			}
			break;
	}

	return false;
}
