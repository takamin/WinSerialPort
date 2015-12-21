#include <windows.h>
#include <string>
#pragma once
//#define OVERLAPPED_IO
class __declspec(dllexport) SerialPort
{
public:
	enum Parity {
		ParityNone = NOPARITY,
		ParityOdd = ODDPARITY,
		ParityEven = EVENPARITY,
		ParityMark = MARKPARITY,
		ParitySpace = SPACEPARITY,
	};
	enum StopBit {
		StopBitOne = ONESTOPBIT,
		StopBitOnePointFive = ONE5STOPBITS,
		StopBitTwo = TWOSTOPBITS,
	};
public:
	SerialPort(void);
	virtual ~SerialPort(void);
	
	void SetPortName(std::string portName) {this->portName = portName;}
	void SetBaudrate(int baudrate) {this->baudrate = baudrate;}
	int GetBaudRate() const { return baudrate; }
	void SetParity(Parity parity) {this->parity = parity;}
	void SetStopBits(StopBit stopbits) {this->stopbits = stopbits;}
	void SetDataBits(int databits) {this->databits = databits;}

	bool Open(int timeout = 0);
	void Close();
	bool IsOpen() const {return (hRs232c != INVALID_HANDLE_VALUE); }
	
	int Read(void* buf, int offset, int count);
	int Write(const void* buf, int offset, int count);

private:
#pragma warning(disable:4251)
	std::string portName;
#pragma warning(default:4251)
	int baudrate;
	Parity parity;
	StopBit stopbits;
	int databits; // 4,5,6,7,8
	bool opened;

	HANDLE hRs232c;
#ifdef OVERLAPPED_IO
	    OVERLAPPED ovRead;
#endif
	char SndBuf[4096];
    char RcvBuf[4096];
};

