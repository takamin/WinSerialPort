#include <iostream>
#include <time.h>
#include <cassert>
#include "TimerThread.h"

TimerThread::TimerThread()
: thead_handle(INVALID_HANDLE_VALUE),
stop_event(INVALID_HANDLE_VALUE), interval(0)
{
}
TimerThread::~TimerThread()
{

#ifdef _DEBUG

	std::deque<long>::iterator t = clock_log.begin();
	double dur_avg = 0.0;
	clock_t prevT = -1;
	std::cerr
		<< "---- " << std::endl
		<< "* execution interval recent " << clock_log.size()
		<< " times: " << std::endl;
	for (; t != clock_log.end(); t++) {
		clock_t T = *t;
		if (prevT >= 0) {
			std::cerr << "+" << (T - prevT) << "/";
			dur_avg += (T - prevT);
		}
		prevT = T;
	}
	std::cerr << std::endl;
	if (clock_log.size() > 1) {
		dur_avg /= (clock_log.size() - 1);
		std::cerr
			<< "* average interval: " << dur_avg << "[ms], "
			<< (1000.0 / dur_avg) << "times/s"
			<< std::endl;
	}

#endif

}
void TimerThread::setInterval(int interval)
{
	assert(interval > 0);
	this->interval = interval;
}
void TimerThread::start()
{
	assert(stop_event == INVALID_HANDLE_VALUE);
	assert(thead_handle == INVALID_HANDLE_VALUE);

	//create event to request the thread termination  
	stop_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(stop_event != NULL);

	this->interval = interval;
	thead_handle = ::CreateThread(0, 0,
		TimerThread::ThreadMain, (LPVOID)this,
		0, NULL);
	assert(thead_handle != NULL);
}
void TimerThread::stop()
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

DWORD WINAPI TimerThread::ThreadMain(LPVOID data)
{
	TimerThread* thread = (TimerThread*)data;
	clock_t sleep_t = 0;			//total waiting time
	long run_counter = 0x7fffffff;	//execution counter
	clock_t t0 = 0;					//a time at first execution
	
	while (::WaitForSingleObject(thread->stop_event, 0) != WAIT_OBJECT_0) {
		
		//reset a counter
		if (run_counter > 0x7ffffff0) {
			run_counter = 0;
			t0 = clock();
		}

		//execute a process
		thread->run();
		
		run_counter++;
		
		//wait
		sleep_t += ((t0 + run_counter * thread->interval) - clock());
		if (sleep_t > 0) {
			Sleep(sleep_t);
			sleep_t = 0;
		}
		else {
			Sleep(1);
			sleep_t -= 1;
		}

#ifdef _DEBUG

		//measure execution elapse
		clock_t t = clock();
		thread->clock_log.push_back(t);
		if (thread->clock_log.size() > 10000) {
			thread->clock_log.pop_front();
		}
		
#endif

	}
	::ExitThread(0);
	return 0;
}
