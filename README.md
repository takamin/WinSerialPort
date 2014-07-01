WinSerialPort
=============

## description

class SerialPort - The class to communicate by serial port
implemented by VisualStudio 2010 Visual C++.

* [SerialPort.h](https://github.com/takamin/WinSerialPort/blob/master/include/SerialPort.h)
* [SerialPort.cpp](https://github.com/takamin/WinSerialPort/blob/master/src/WinSerialPort/SerialPort.cpp)

### public members

#### constructor

* SerialPort();

#### set / get conditions

* void SetPortName(std::string portName);
* void SetBaudrate(int baudrate);
* int GetBaudRate() const;
* void SetParity(Parity parity);
* void SetStopBits(StopBit stopbits);
* void SetDataBits(int databits);

#### open / close

* bool Open(int timeout = 0);
* void Close();
* bool IsOpen() const;

#### input / output

* int Read(void* buf, int offset, int count);
* int Write(const void* buf, int offset, int count);

#### enum Parity

* ParityNone
* ParityOdd
* ParityEven
* ParityMark
* ParitySpace

#### enum StopBit

* StopBitOne
* StopBitOnePointFive
* StopBitTwo
