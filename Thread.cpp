#include <iostream>
#include <time.h>
#include <cassert>
#include "Thread.h"

Thread::Thread()
: thead_handle(INVALID_HANDLE_VALUE),
stop_event(INVALID_HANDLE_VALUE), interval(0)
{
}
Thread::~Thread()
{
}
void Thread::setInterval(int interval)
{
	assert(interval > 0);
	this->interval = interval;
}
void Thread::start()
{
	assert(stop_event == INVALID_HANDLE_VALUE);
	assert(thead_handle == INVALID_HANDLE_VALUE);

	//create event to request the thread termination  
	stop_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(stop_event != NULL);

	this->interval = interval;
	thead_handle = ::CreateThread(0, 0,
		Thread::ThreadMain, (LPVOID)this,
		0, NULL);
	assert(thead_handle != NULL);
}
void Thread::stop()
{
	assert(stop_event != NULL);
	assert(thead_handle != NULL);

	::SetEvent(stop_event);
	
	WaitForSingleObject(thead_handle, INFINITE);
	::CloseHandle(thead_handle);
	thead_handle = INVALID_HANDLE_VALUE;

	::CloseHandle(stop_event);
	stop_event = INVALID_HANDLE_VALUE;
}

DWORD WINAPI Thread::ThreadMain(LPVOID data)
{
	Thread* thread = (Thread*)data;
	while (::WaitForSingleObject(thread->stop_event, 0) != WAIT_OBJECT_0) {
		if (!thread->run()) {
			break;
		}
	}
	::ExitThread(0);
	return 0;
}
