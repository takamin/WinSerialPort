#pragma once
#include <windows.h>
#include "Thread.h"
#include "SerialPort.h"

class SerialPortReceiverThread : public Thread {
public:
	typedef void(*ON_BYTE_RECEIVED)(unsigned char data);
public:
	SerialPortReceiverThread(SerialPort& serialPort, ON_BYTE_RECEIVED onByteReceived);
protected:
	virtual bool run();
private:
	SerialPort& serialPort;
	ON_BYTE_RECEIVED onByteReceived;
};
