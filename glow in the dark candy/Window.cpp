#include "Window.h"
#include <mfapi.h>
#include <mfidl.h>
//#include <dwmapi.h>
#include <bitset>
#include <list>

#include <sys/stat.h>

//#pragma comment(lib, "dwmapi.lib")
//#pragma comment(lib, "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Lib\\x64\\dwmapi.lib")

//#define USE_TOUCHPAD
#ifdef USE_TOUCHPAD
#include "SynKit.h"
#pragma comment(lib, "C:\\Users\\Tiago\\Documents\\Visual Studio 2010\\Projects\\xrlshared\\xrlshared\\SynCom.lib")
#endif

static HINSTANCE hinstance;
static HWND hwnd;
static HDC hdc;
static unsigned frame = 0;
static unsigned wndwidth, wndheight, scrwidth, scrheight;
static bool fullscreen = false;
static HMIDIIN hmidiin;
static BYTE knob[128+2];	// CC + program + aftertouch
static float wheel = 0.0f;
static std::bitset<256> keyboard, lastkeyboard;
static long mousex = 0, mousey = 0, mousedx, mousedy;
static int mousewheel = 0;
static bool dwmenabled;
static double aspectratio;
static double currenttime, dtime;

static WORD buttonmask = 0, buttons = 0, lastbuttons = 0;
static unsigned sx1a = 0, sy1a = 0, sx2a = 0, sy2a = 0, ac = 0;
static float sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0;

static char *screenshotbuffer;
static unsigned screenshotpixelbuffer;

const char Window::wndname[] = "xrwgl";

unsigned Window::screen_width;
unsigned Window::screen_height;
double Window::screen_aspect_ratio;


double Window::Time()
{
	return currenttime;
}


double Window::DTime()
{
	return dtime;
}


void Window::Kill()
{
	DestroyWindow(hwnd);
}


double Window::AspectRatio()
{
	return aspectratio;
}


unsigned Window::Width()
{
	return wndwidth;
}


unsigned Window::Height()
{
	return wndheight;
}


unsigned Window::ScreenWidth()
{
	return scrwidth;
}


unsigned Window::ScreenHeight()
{
	return scrheight;
}


HWND Window::WindowH()
{
	return hwnd;
}


unsigned Window::Frame()
{
	return frame;
}


bool Window::CreateFakeContext()
{
	static const int pfattribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		0
	};

	HWND hwnd = CreateWindowEx(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	HDC hdc = GetDC(hwnd);
	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
	HGLRC hglc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglc);

	bool r = InitWGLExtensions();

	unsigned npixelformats;

	if(!r || !wglChoosePixelFormatARB(hdc, pfattribs, 0, 1, &pixel_format, &npixelformats) || npixelformats == 0)
		pixel_format = -1;

	wglMakeCurrent(0, 0);
	wglDeleteContext(hglc);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);

	if(!(r && (pixel_format != -1)))
		return false;

	EnumDisplayMonitors(0, 0, GetScreen, 0);

	hinstance = GetModuleHandle(0);

	WNDCLASSEX wndc =
	{
		sizeof(WNDCLASSEXA),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hinstance,
		0,
		0,
		0,
		0,
		wndname,
		0
	};

	RegisterClassEx(&wndc);

	//new_window_mutex = CreateMutex(0, 0, 0);

	return true;
}


bool Window::Init()
{
	CreateMutexA(0, 0, wndname);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return false;

	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);

	screen_aspect_ratio = (double)screen_width / screen_height;

	wndc.lpfnWndProc = WndProc;
	RegisterClassExA(&wndc);

	hwnd = CreateWindowExA(WS_EX_APPWINDOW, (LPCSTR)wndname, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	hdc = GetDC(hwnd);

	unsigned nPixelFormats;
	int pixelformat;

	static const int iAtt[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		WGL_DEPTH_BITS_ARB, 0,
		WGL_STENCIL_BITS_ARB, 0,
		0
	};

	if(!wglChoosePixelFormatARB(hdc, iAtt, 0, 1, &pixelformat, &nPixelFormats))
		return false;

	SetPixelFormat(hdc, pixelformat, &pfd);

	static const int ctxattrib[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, GL_TRUE,
		0
	};

	hglc = wglCreateContextAttribsARB(hdc, 0, ctxattrib);
	//hglc = wglCreateContext(hdc);

	if(!hglc)
		return false;

	if(!InitOpenGLExtensions())
		return false;

	wglMakeCurrent(hdc, hglc);
	wglSwapIntervalEXT(1);

	return true;
}


#ifdef USE_TOUCHPAD
ISynAPI *pAPI = 0;
long lHandle = -1;
ISynDevice *pDevice = 0;
ISynPacket *packet = 0;
#endif

long minx, maxx, miny, maxy, minz, maxz;
HANDLE hevent;


void Window::InitInput()
{
	RAWINPUTDEVICE rid[3];

	rid[0].usUsagePage = 0x01; 
	rid[0].usUsage = 0x06;		// keyboard
	rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_APPKEYS;//| RIDEV_NOHOTKEYS | RIDEV_APPKEYS;
	rid[0].hwndTarget = hwnd;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x04;		// joystick
	rid[1].dwFlags = RIDEV_EXINPUTSINK | RIDEV_INPUTSINK;
	rid[1].hwndTarget = hwnd;

	rid[2].usUsagePage = 0x01; 
	rid[2].usUsage = 0x02;		// mouse
	rid[2].dwFlags = RIDEV_NOLEGACY;//| RIDEV_CAPTUREMOUSE;
	rid[2].hwndTarget = hwnd;

	RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE));
	SetCapture(hwnd);	// alt mouse
}


void Window::InitMidiInput()
{
	midiInOpen(&hmidiin, 0, (DWORD_PTR)hwnd, 0, CALLBACK_WINDOW);
	midiInStart(hmidiin);
}


#ifdef USE_TOUCHPAD
void Window::InitTouchPad()
{
	SynCreateAPI(&pAPI);

	if(pAPI->FindDevice(SE_ConnectionAny, SE_DeviceTouchPad, &lHandle) == SYN_OK && pAPI->CreateDevice(lHandle, &pDevice) == SYN_OK)
	{
		pDevice->GetProperty(SP_XHiBorder, &maxx);
		pDevice->GetProperty(SP_XLoBorder, &minx);

		pDevice->GetProperty(SP_YHiBorder, &maxy);
		pDevice->GetProperty(SP_YLoBorder, &miny);

		pDevice->GetProperty(SP_ZMaximum, &maxz);
		pDevice->GetProperty(SP_ZTouchThreshold, &minz);

		pDevice->CreatePacket(&packet);

		hevent = CreateEvent(0, 0, 0, 0);
		pDevice->SetEventNotification(hevent);
	}
}


void Window::ReadTouchPad()
{
	long x, y, z;

	if(WaitForSingleObject(hevent, 0) == WAIT_OBJECT_0)
		while(pDevice->LoadPacket(packet) != SYNE_FAIL)
		{
			//packet->GetProperty(SP_W, &w);	// width
			packet->GetProperty(SP_X, &x);
			packet->GetProperty(SP_Y, &y);
			packet->GetProperty(SP_Z, &z);	// pressure

			double touchpadx = (double)(x-minx) / (maxx-minx);
			double touchpady = (double)(x-minx) / (maxx-minx);
			double touchpadz = (double)(x-minx) / (maxx-minx);
		}
}
#endif


void Window::SwitchMode()
{
	FullScreen(!fullscreen);
}


void Window::FullScreen(bool fs)
{
	if(!fs)
	{
		wndwidth = scrwidth/2;
		wndheight = scrheight/2;
		fullscreen = false;
		
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_CAPTION | WS_VISIBLE);
		SetWindowPos(hwnd, 0, (scrwidth-wndwidth)/2, (scrheight-wndheight)/2, wndwidth, wndheight, SWP_ASYNCWINDOWPOS | SWP_DEFERERASE | SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOCOPYBITS | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_SHOWWINDOW);

		RECT rect;
		GetClientRect(hwnd, &rect);
		wndwidth = rect.right;
		wndheight = rect.bottom;
	}
	else if(fs)
	{
		wndwidth = scrwidth;
		wndheight = scrheight;
		fullscreen = true;

		SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE);
		SetWindowPos(hwnd, 0, 0, 0, scrwidth, scrheight, SWP_ASYNCWINDOWPOS | SWP_DEFERERASE | SWP_FRAMECHANGED | SWP_NOCOPYBITS | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_SHOWWINDOW);
	}

	aspectratio = (float)wndwidth / wndheight;

	glViewport(0, 0, wndwidth, wndheight);
}


DWORD WINAPI Window::WindowThread(void (*Update)())
{
	wglMakeCurrent(hdc, hglc);

	static unsigned __int64 cfreq, currc, offsetc, lastc;
	static double lasttime;
	static long lastmx, lastmy;
	static MSG msg;

	buttons = sx1a = sy1a = sx2a = sy2a = buttonmask = 0;
	keyboard.reset();

	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&cfreq));
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&offsetc));

	for(;;)
	{
		// keyboard
		lastkeyboard = keyboard;

		// mouse
		lastmx = mousex;
		lastmy = mousey;

		// gamepad
		lastbuttons = buttons;
		buttons = buttonmask;
		sx1 = ((float)sx1a/ac-128.0f)/127.0f;
		sy1 = ((float)sy1a/ac-128.0f)/-127.0f;
		sx2 = ((float)sx2a/ac-128.0f)/127.0f;
		sy2 = ((float)sy2a/ac-128.0f)/-127.0f;
		sx1a = sy1a = sx2a = sy2a = ac = buttonmask = 0;

		// time
		lasttime = currenttime;
		lastc = currc;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currc));
		currenttime = (double)(currc-offsetc)/(double)cfreq;
		dtime = (double)(currc-lastc)/(double)cfreq;

		mousedx = mousex - lastmx;
		mousedy = mousey - lastmy;

		Update();

		frame++;
		SwapBuffers(hdc);
	}

	return msg.wParam;

}

WPARAM Window::Loop(void (*Update)())
{
	static MSG msg;

	do
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	while(msg.message != WM_QUIT);

	return msg.wParam;
}


LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	switch(msg)
	{
	case WM_SYSCOMMAND:
		switch(wParam)
		{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
		}
		break;

	case WM_KILLFOCUS:
		//Pause();
		return 0;

	case WM_SETFOCUS:
		//Resume();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case MM_MIM_DATA:	// MIDI in data!
	{
		//static BYTE channel = lParam & 0x0f;		// MIDI channel
		static BYTE message = (lParam>>4) & 0x0f;	// MIDI message
		static BYTE byte1 = (BYTE)(lParam>>8);		// MIDI byte 1
		static BYTE byte2 = (BYTE)(lParam>>16);		// MIDI byte 2

		if(message==0x0B)	// MIDI CC
			knob[byte1] = byte2;

		if(message==0x0C)	// Program
			knob[128] = byte1;
			
		if(message==0x0D)	// Aftertouch
			knob[129] = byte1;

		if(message==0x0E)
			wheel = (float)(byte2<<7|byte1)/8191.5f-1.0f;

		return 0;
	}

	case WM_INPUT:
	{
		static UINT datasize = 64;
		static BYTE rawdata[64];

		if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawdata, &datasize, sizeof(RAWINPUTHEADER)))
		{
			switch(((RAWINPUT*)rawdata)->header.dwType)
			{
			case RIM_TYPEHID:	// joystick
			{
				sx1a += ((RAWINPUT*)rawdata)->data.hid.bRawData[1];
				sy1a += ((RAWINPUT*)rawdata)->data.hid.bRawData[2];
				sx2a += ((RAWINPUT*)rawdata)->data.hid.bRawData[4];
				sy2a += ((RAWINPUT*)rawdata)->data.hid.bRawData[5];
				++ac;

				WORD button = (((RAWINPUT*)rawdata)->data.hid.bRawData[7] << 8) | ((RAWINPUT*)rawdata)->data.hid.bRawData[6];
				buttonmask = (buttonmask & B_MASK) | button;
			}

			case RIM_TYPEKEYBOARD:
			{
				if(((RAWINPUT*)rawdata)->data.keyboard.Flags & RI_KEY_BREAK)	// up
					keyboard[((RAWINPUT*)rawdata)->data.keyboard.VKey] = false;
				else
					keyboard[((RAWINPUT*)rawdata)->data.keyboard.VKey] = true;
			}

			case RIM_TYPEMOUSE:
			{
				if(((RAWINPUT*)rawdata)->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					mousex = ((RAWINPUT*)rawdata)->data.mouse.lLastX;
					mousey = ((RAWINPUT*)rawdata)->data.mouse.lLastY;
				}
				else
				{
					mousex += ((RAWINPUT*)rawdata)->data.mouse.lLastX;
					mousey += ((RAWINPUT*)rawdata)->data.mouse.lLastY;
				}

				if(((RAWINPUT*)rawdata)->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
					mousewheel += (SHORT)((RAWINPUT*)rawdata)->data.mouse.usButtonData;
			}
			}
		}

		return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


bool Window::KeyPressed(const USHORT &k)
{
	/*unsigned idx = k / 64;
	unsigned __int64 mask = 1 << (k % 64);

	return keyboard[idx] & mask;*/

	return keyboard[k];
}


bool Window::KeyDown(const USHORT &k)
{
	/*unsigned idx = k / 64;
	unsigned __int64 mask = 1 << (k % 64);

	return !(lastkeyboard[idx] & mask) && (keyboard[idx] & mask);*/

	return !lastkeyboard[k] && keyboard[k];
}


bool Window::KeyUp(const USHORT &k)
{
	/*unsigned idx = k / 64;
	unsigned __int64 mask = 1 << (k % 64);

	return (lastkeyboard[idx] & mask) && !(keyboard[idx] & mask);*/

	return lastkeyboard[k] && !keyboard[k];
}


void* Window::LoadFile(const char* filename, int* len)
{
	HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

	if(file == INVALID_HANDLE_VALUE)
		return 0;

	unsigned filesize = GetFileSize(file, 0);

	void *buffer = (void*)new unsigned char[filesize];

	unsigned bytesread;

	if(!ReadFile(file, buffer, filesize, (LPDWORD)&bytesread, 0))
		bytesread = -1;

	CloseHandle(file);
	
	if(bytesread == filesize)
	{
		if(len)
			*len = bytesread;
		
		return buffer;
	}
	else
	{
		delete [] buffer;
		return 0;
	}
}


bool Window::ButtonPressed(const Buttons &b)
{
	return (buttons & b) != 0;
}


bool Window::ButtonUp(const Buttons &b)
{
	return (lastbuttons & b) && !(buttons & b);
}

bool Window::ButtonDown(const Buttons &b)
{
	return !(lastbuttons & b) && (buttons & b);
}

float Window::Stick1X()
{
	return sx1;
}

float Window::Stick1Y()
{
	return sy1;
}

float Window::Stick2X()
{
	return sx2;
}

float Window::Stick2Y()
{
	return sy2;
}

bool Window::IsFullScreen()
{
	return fullscreen;
}
