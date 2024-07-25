#include "stb_vorbis.h"
#include <Windows.h>

#pragma comment(lib, "winmm.lib")


class OGGPlayer
{
public:

	static bool Init(const char* filepath);
	static void Kill();

	static void Play();
	static void Pause();

private:

	static void InitSound();
	static DWORD WINAPI SoundThread(LPVOID lpParameter);
};
