#pragma once

#include <windows.h>

class ThreadManager
{
public:

	typedef void (*function)(void *);

	static void Init(bool suspended, int n = -1);
	static void Kill();

	static void Push(function f, void *p);
	static void Work();
	static void WorkAndWait();

	static int RunningJobs(){ return runningjobs; };
	static int QueuedJobs(){ return queuedjobs; };
	static unsigned NProcessors(){ return nprocessors; };
	static unsigned NThreads(){ return nthreads; };

	static void Suspend();
	static void Resume();

private:

	typedef struct NODE
	{
		function f;
		void* p;
		NODE* next;
	} *node;

	static DWORD ThreadFunction(unsigned nthread);

	static node listhead, listtail;
	static int runningjobs, queuedjobs;
	static unsigned nprocessors, nthreads;
	static HANDLE mutex;
	static HANDLE* threadid;
};
