#pragma once
#include <windows.h>
#include "Thread.h"
#include "SerialPort.h"
#include "C99_types.h"

class SerialPortReceiverThread : public Thread {
public:
	typedef void(*ON_BYTE_RECEIVED)(unsigned char data, SerialPort& serialPort);
public:
	SerialPortReceiverThread(SerialPort& serialPort, ON_BYTE_RECEIVED onByteReceived);
protected:
	virtual bool run();
private:
	SerialPort& serialPort;
	ON_BYTE_RECEIVED onByteReceived;
};
