#include <iostream>
#include <time.h>
#include <cassert>
#include "SerialPortReceiverThread.h"

SerialPortReceiverThread::SerialPortReceiverThread(
	SerialPort& serialPort, ON_BYTE_RECEIVED onByteReceived)
	: serialPort(serialPort), onByteReceived(onByteReceived)
{

}

bool SerialPortReceiverThread::run()
{
	unsigned char data;
	if (serialPort.IsOpen()) {
		if (serialPort.Read(&data, 0, 1) == 1) {
			this->onByteReceived(data, serialPort);
		}
		else {
			Sleep(10);
		}
	}
	return true;
}
