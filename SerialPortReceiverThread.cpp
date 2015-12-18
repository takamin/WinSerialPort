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
	uint8_t data;
	if (serialPort.IsOpen()) {
		if (serialPort.Read(&data, 0, 1)) {
			this->onByteReceived(data);
		}
		else {
			Sleep(10);
		}
	}
	return true;
}
