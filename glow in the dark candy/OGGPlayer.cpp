#include "OGGPlayer.h"
#include "Window.h"
#include <Mmreg.h>


static void* file;
static stb_vorbis* stream;

static stb_vorbis_info ogginfo;

static unsigned threadid;

static WAVEHDR wavehdr;
static HWAVEOUT waveout;

float *soundbuffer;

enum
{
	BUFLEN = 8192 * 2,
	//samplerate = 48000
};


bool OGGPlayer::Init(const char* filepath)
{
	unsigned filesize;
	
	file = Window::LoadFile(filepath, &filesize);
	if(!file)
		return false;

	int error;

	stream = stb_vorbis_open_memory((unsigned char*)file, filesize, &error, 0);
	if(!stream)
	{
		delete [] file;
		return false;
	}

	ogginfo = stb_vorbis_get_info(stream);

	soundbuffer = new float[4 * BUFLEN];
	stb_vorbis_get_samples_float_interleaved(stream, 2, soundbuffer, 2*BUFLEN);

	InitSound();

	CreateThread(0, 0, SoundThread, 0, 0, (LPDWORD)&threadid);

	return true;
}


void OGGPlayer::Kill()
{
	waveOutClose(waveout);

	CloseHandle((HANDLE)threadid);

	delete [] soundbuffer;
	delete [] file;
}


void OGGPlayer::Play()
{
	waveOutRestart(waveout);
}


void OGGPlayer::Pause()
{
	waveOutPause(waveout);
}


void OGGPlayer::InitSound()
{	
	wavehdr.dwFlags			= WHDR_BEGINLOOP|WHDR_ENDLOOP;
	wavehdr.lpData			= (LPSTR)soundbuffer;
	wavehdr.dwBufferLength	= sizeof(float) * 4 * BUFLEN;
	wavehdr.dwLoops			= -1;

	WAVEFORMATEX pcmwf;
	pcmwf.wFormatTag		= WAVE_FORMAT_IEEE_FLOAT;
	pcmwf.nChannels			= 2;
	pcmwf.nSamplesPerSec	= ogginfo.sample_rate;//samplerate;
	pcmwf.wBitsPerSample	= 32;
	pcmwf.nBlockAlign		= pcmwf.nChannels*pcmwf.wBitsPerSample / 8;
	pcmwf.nAvgBytesPerSec	= pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
	pcmwf.cbSize			= 0;

	waveOutOpen(&waveout, 0, &pcmwf, 0, 0, WAVE_ALLOWSYNC | WAVE_FORMAT_DIRECT|CALLBACK_NULL);
	waveOutPause(waveout);
	waveOutPrepareHeader(waveout, &wavehdr, sizeof(WAVEHDR));
	waveOutWrite(waveout, &wavehdr, sizeof(WAVEHDR));
}


DWORD WINAPI OGGPlayer::SoundThread(LPVOID lpParameter)
{
	int lastbuf = 1;

	MMTIME mmtime =
	{
		TIME_SAMPLES,
		0
	};

	while(true)
	{	
		waveOutGetPosition(waveout, &mmtime, sizeof(MMTIME));

		int curbuf = (mmtime.u.sample / BUFLEN) % 2;	// currently playing buffer

		if(curbuf != lastbuf)
		{
			float *buffer = &soundbuffer[lastbuf * BUFLEN * 2];

			unsigned n = stb_vorbis_get_samples_float_interleaved(stream, 2, buffer, 2*BUFLEN);

			if(n < BUFLEN)
			{
				SecureZeroMemory(buffer+n, sizeof(float)*2*(BUFLEN-n));

				if(curbuf == 0)
					break;
			}
		}

		lastbuf = curbuf;

		Sleep(BUFLEN * 1000 / 2 / ogginfo.sample_rate);
	}

	waveOutBreakLoop(waveout);
	stb_vorbis_close(stream);

	threadid = 0;

	return 0;
}
