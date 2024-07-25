#pragma once

#pragma comment(lib, "winmm.lib")
#include "OpenGL.h"
#include "stb_image.h"

enum Buttons
{
	B_R3 = 0x8000,
	B_L3 = 0x4000,
	B_START = 0x2000,
	B_SELECT = 0x1000,
	B_R1 = 0x0800,
	B_L1 = 0x0400,
	B_R2 = 0x0200,
	B_L2 = 0x0100,
	B_4 = 0x0080,
	B_3 = 0x0040,
	B_2 = 0x0020,
	B_1 = 0x0010,
	B_DU = 0x0000,
	B_DUR = 0x0001,
	B_DR = 0x0002,
	B_DDR = 0x0003,
	B_DD = 0x0004,
	B_DL = 0x0005,
	B_L = 0x0006,
	B_UL = 0x0007,
	B_D0 = 0x0008,
	B_MASK = 0xFFF0,
	B_DPAD = 0x000F
};


class Window
{
public:

	static WPARAM Loop(void (*Update)());
	static bool Init();
	static void Kill();
	static void FullScreen(bool fs);
	static void SwitchMode();
	static double AspectRatio();
	static unsigned Width();
	static unsigned Height();
	static unsigned ScreenWidth();
	static unsigned ScreenHeight();
	static unsigned Frame();
	static double Time();
	static double DTime();
	static bool IsFullScreen();

	static void InitInput();
	static void InitTouchPad();
	static void InitMidiInput();

	static HWND WindowH();

	static void* LoadFile(const char* filename, int* len);

	static bool KeyUp(const USHORT &k);
	static bool KeyDown(const USHORT &k);
	static bool KeyPressed(const USHORT &k);
	static bool ButtonPressed(const Buttons &b);
	static bool ButtonUp(const Buttons &b);
	static bool ButtonDown(const Buttons &b);
	static float Stick1X();
	static float Stick1Y();
	static float Stick2X();
	static float Stick2Y();
	
	static void ReadTouchPad();

private:

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static bool CreateFakeContext();
	static DWORD WINAPI WindowThread(void (*Update)());

	static const char wndname[];

	static unsigned screen_width;
	static unsigned screen_height;
	static double screen_aspect_ratio;
};
