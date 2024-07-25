#pragma once

#include <Windows.h>

class Timer
{
public:

	Timer();
	void Reset();
	void Pause();
	void UnPause();
	float fGet();
	double dGet();
	__int64 iGet();

private:

	__int64 zero, aux;
	static __int64 freq;
};
