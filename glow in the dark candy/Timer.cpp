#include "Timer.hpp"

__int64 Timer::freq;

Timer::Timer()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
	Reset();
}

void Timer::Reset()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&zero));
}

void Timer::Pause()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&aux));
}

void Timer::UnPause()
{
	zero -= aux;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&aux));
	zero += aux;
}

__int64 Timer::iGet()
{
	__int64 t;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&t));
	return t-zero;
}

float Timer::fGet()
{
	return (float)iGet()/freq;
}

double Timer::dGet()
{
	return (double)iGet()/freq;
}
