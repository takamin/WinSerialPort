cmake_minimum_required(VERSION 2.8)
project( WinSerialPort )
add_library(WinSerialPort
    SerialPort.cpp
    Thread.cpp
    SerialPortReceiverThread.cpp
    CRC16CCITT.cpp
    hexdump.cpp
)
