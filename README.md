WinSerialPort
=============

C++ SerialPort class for Microsoft Visual C++.

## Source Files

* [SerialPort.h](https://github.com/takamin/WinSerialPort/blob/master/SerialPort.h)
* [SerialPort.cpp](https://github.com/takamin/WinSerialPort/blob/master/SerialPort.cpp)

### Public Members

#### Constructor

* SerialPort();

#### Set / Get conditions

* void SetPortName(std::string portName);
* void SetBaudrate(int baudrate);
* int GetBaudRate() const;
* void SetParity(Parity parity);
* void SetStopBits(StopBit stopbits);
* void SetDataBits(int databits);

#### Open / Close

* bool Open(int timeout = 0);
* void Close();
* bool IsOpen() const;

#### Input / Output

* int Read(void\* buf, int offset, int count);
* int Write(const void\* buf, int offset, int count);

#### Enumeration Values for Parity

* ParityNone
* ParityOdd
* ParityEven
* ParityMark
* ParitySpace

#### Enumeration Values for StopBit

* StopBitOne
* StopBitOnePointFive
* StopBitTwo
