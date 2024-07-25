#include "ThreadManager.h"

//#define SINGLE_THREAD
#define USE_SLEEP

#include <stdio.h>


ThreadManager::node ThreadManager::listhead, ThreadManager::listtail;
int ThreadManager::runningjobs, ThreadManager::queuedjobs;
unsigned ThreadManager::nprocessors, ThreadManager::nthreads;
HANDLE ThreadManager::mutex;
HANDLE* ThreadManager::threadid;


void ThreadManager::Init(bool suspended, int n)
{
#ifndef SINGLE_THREAD

	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	nprocessors = sysinfo.dwNumberOfProcessors;

	nthreads = nprocessors + n;
	nthreads = nthreads <= 0 ? 1 : nthreads;

	threadid = new HANDLE[nthreads];

	queuedjobs = 0;
	runningjobs = 0;
	listhead = listtail = 0;

	mutex = CreateMutex(0, 0, 0);

	for(unsigned i=0; i<nthreads; ++i)
		threadid[i] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadFunction, (LPVOID)(i+1), suspended?CREATE_SUSPENDED:0, 0);

#endif
}


void ThreadManager::Kill()
{
#ifndef SINGLE_THREAD

	delete [] threadid;
	runningjobs = -1;

#endif
}


DWORD ThreadManager::ThreadFunction(unsigned nthread)
{
	while(runningjobs>=0)
	{
		Work();

#ifdef USE_SLEEP
		Sleep(0);
#endif
	}

	return 0;
}


void ThreadManager::Work()
{
#ifndef SINGLE_THREAD

start:
	WaitForSingleObject(mutex, INFINITE);
	if(queuedjobs)
	{
		node t = listhead;
		
		if(listtail == listhead)
			listtail = listhead = 0;
		else
			listhead = listhead->next;

		queuedjobs--;

		ReleaseMutex(mutex);

		runningjobs++;
		t->f(t->p);
		runningjobs--;

		delete t;

		goto start;
	}
	else
		ReleaseMutex(mutex);

#endif
}


void ThreadManager::WorkAndWait()
{
#ifndef SINGLE_THREAD

	while(runningjobs)
	{
		Work();

#ifdef USE_SLEEP
		Sleep(0);
#endif
	}

#endif
}


void ThreadManager::Push(function f, void* p)
{
#ifdef SINGLE_THREAD

	f(p);

#else
	
	node newnode = new struct NODE;
	newnode->f = f;
	newnode->p = p;
	newnode->next = 0;

	WaitForSingleObject(mutex, INFINITE);

	if(listtail)
		listtail = listtail->next = newnode;
	else
		listtail = listhead = newnode;

	queuedjobs++;

	ReleaseMutex(mutex);

#endif
}


void ThreadManager::Suspend()
{
#ifndef SINGLE_THREAD

	for(unsigned i=0; i<nthreads; ++i)
		SuspendThread(threadid[i]);

#endif
}


void ThreadManager::Resume()
{
#ifndef SINGLE_THREAD

	for(unsigned i=0; i<nthreads; ++i)
		ResumeThread(threadid[i]);

#endif
}
