#include <iostream>
#include <time.h>
#include "SerialPort.h"

using namespace std;

SerialPort::SerialPort()
	:
	portName(""),
	baudrate(9600),
	parity(ParityNone),
	stopbits(StopBitOne),
	databits(8),
	flowCtrl(FlowCtrlNone),
	hRs232c(INVALID_HANDLE_VALUE)
{
}
SerialPort::~SerialPort()
{
}
bool SerialPort::Open(int timeout/* = 0*/)
{
    memset(SndBuf,0,sizeof(SndBuf));
    memset(RcvBuf,0,sizeof(RcvBuf));

	string filename = string("\\\\.\\").append(portName);

    // COMポートのオープン
    hRs232c = CreateFile(
        filename.c_str(),
		GENERIC_READ|GENERIC_WRITE,0,NULL,
        OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL
#ifdef OVERLAPPED_IO
		|FILE_FLAG_OVERLAPPED
#endif
		,NULL);
    if ( hRs232c == INVALID_HANDLE_VALUE ) {
#ifdef _DEBUG
        cerr << "RS232C CreateFile Error" << endl;
#endif
        return false;
    }


    // COMポートのセットアップ
    if (SetupComm(hRs232c,sizeof(RcvBuf),sizeof(SndBuf)) == FALSE){
#ifdef _DEBUG
        cerr << "RS232C SetupComm Error" << endl;
#endif
        return false;
    }
	
    // COMポートの設定取得
    DCB dcb;
    memset(&dcb, NULL, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    GetCommState(hRs232c, &dcb );

    // COMポートの設定変更
    dcb.BaudRate = baudrate;
    dcb.Parity   = (int)parity;
    dcb.StopBits = (int)stopbits;
    dcb.ByteSize = databits;

	// Flow control
	switch (this->flowCtrl) {
		break;
	case FlowCtrlXonXoff:
		dcb.fInX = TRUE;
		dcb.fOutX = TRUE;
		dcb.XonChar = 0x11;
		dcb.XoffChar = 0x13;
		dcb.XonLim = 0;
		dcb.XoffLim = 0;
		break;
	case FlowCtrlHardware:
		dcb.fOutxCtsFlow = TRUE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcb.XonLim = 0;
		dcb.XoffLim = 0;
		break;
	case FlowCtrlNone:
	default:
		break;
	}
    if (SetCommState(hRs232c,&dcb) == FALSE){
#ifdef _DEBUG
        cerr << "RS232C SetCommState Error" << endl;
#endif
        return false;
    }
	COMMTIMEOUTS tmo;
	tmo.ReadTotalTimeoutConstant = timeout;
	tmo.ReadTotalTimeoutMultiplier = 0;
	tmo.WriteTotalTimeoutConstant = timeout;
	tmo.WriteTotalTimeoutMultiplier = 0;
	if(SetCommTimeouts(hRs232c, &tmo) == FALSE) {
#ifdef _DEBUG
        cerr << "RS232C SetCommTimeouts Error" << endl;
#endif
		return false;
	}

#ifdef OVERLAPPED_IO
    // COMポートの受信イベント作成
    memset(&ovRead,0,sizeof(ovRead));
    ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ovRead.hEvent == NULL){
#ifdef _DEBUG
        cerr << "RS232C CreateEvent Error" << endl;
#endif
        return 0;
    }
    // COMポートの受信イベントマスク
    if (SetCommMask(hRs232c, EV_RXCHAR) == FALSE){
#ifdef _DEBUG
        cerr << "RS232C SetCommMask Error" << endl;
#endif
        return 0;
    }
#endif
	return true;
}
void SerialPort::Close()
{
#ifdef OVERLAPPED_IO
    // OMポートの受信イベントクローズ
    CloseHandle(ovRead.hEvent);
#endif

    // COMポートのクローズ
    CloseHandle(hRs232c);
	hRs232c = INVALID_HANDLE_VALUE;
}
int SerialPort::Write(const void* buf, int offset, int count)
{
	const unsigned char* sendbuf = (const unsigned char*)buf;
    DWORD dwSize = 0;
	OVERLAPPED* pOverlapped = 0;

#ifdef OVERLAPPED_IO
    OVERLAPPED ovWrite;
    memset(&ovWrite,0,sizeof(ovWrite));
	pOverlapped = &ovWrite;
#endif

    if (WriteFile(hRs232c, &sendbuf[offset], count, &dwSize, pOverlapped) == FALSE) {
#ifdef _DEBUG
        cerr << "RS232C WriteFile Error" << endl;
#endif
        return 0;
    }
	return (int)dwSize;
}
int SerialPort::Read(void* buf, int offset, int count)
{
	unsigned char* recvbuf = (unsigned char*)buf;
	DWORD dwSize = 0;
	OVERLAPPED* pOverlapped = 0;

#ifdef OVERLAPPED_IO
	pOverlapped = &ovRead;

    // COMポートの受信待ち
    DWORD dwEvtMask = 0;
    DWORD dwTransfer = 0;
	clock_t wait_start = clock();
    if (WaitCommEvent(hRs232c, &dwEvtMask, pOverlapped) == FALSE){
        if (GetLastError() == ERROR_IO_PENDING) {
            GetOverlappedResult(hRs232c, pOverlapped, &dwTransfer, TRUE);
        }else{
#ifdef _DEBUG
        cerr << "RS232C WaitCommEvent Error" << endl;
#endif
            return 0;
        }
    }
    // COMポートの受信イベントチェック
    if ((dwEvtMask & EV_RXCHAR) != EV_RXCHAR){
		return 0;
	}
    // COMポートの受信データチェック
    COMSTAT comst;
    DWORD dwErr;
    ClearCommError(hRs232c, &dwErr, &comst);
	if(comst.cbInQue < count) {
		return 0;
	}
#endif
	// Check available byte count
	DWORD dwError = 0UL;
	COMSTAT comstat;
	ClearCommError(hRs232c, &dwError, &comstat);
	DWORD dwCount = comstat.cbInQue;
	if (dwCount < count) {
		return 0;
	}
    // COMポートからデータ受信
    if(ReadFile(hRs232c, &recvbuf[offset], count, &dwSize, pOverlapped) == FALSE){
#ifdef _DEBUG
        cerr << "RS232C ReadFile Error" << endl;
#endif
        return -1;
    }
	if(count != 0 && dwSize == 0) {
		return -1;
	}
	return (int)dwSize;
}
