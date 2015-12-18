#pragma once
#include <windows.h>

#ifdef _DEBUG

#include <deque>

#endif

class Thread {
public:
	Thread();
	virtual ~Thread();
	void setInterval(int interval);
	void start();
	void stop();
protected:
	virtual bool run() = 0;
	
protected:
	HANDLE stop_event;
	HANDLE thead_handle;
	int interval;
	
private:
	static DWORD WINAPI ThreadMain(LPVOID data);
	
#ifdef _DEBUG

private:
	std::deque<long> clock_log;

#endif

};
