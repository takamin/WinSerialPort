#include "CRC16CCITT.h"

/*
 * http://web.mit.edu/6.115/www/miscfiles/amulet/amulet-help/xmodem.htm
 */
void CRC16CCITT::calc_crc(int c) {
    int count;
    crc ^= (c << 8);
    for (count=8; --count>=0;) {
        if ((crc & 0x8000) != 0) {
            crc <<= 1;
            crc ^= 0x1021;
        } else {
            crc <<= 1;
        }
    }
}

/*
 * http://www.techfest.com/hardware/modem/xymodem.htm
 */
/*****************************************************
void CRC16CCITT::calc_crc(int c) {
    int count;
    for (count=8; --count>=0;) {
        if ((crc & 0x8000) != 0) {
            crc <<= 1;
            crc += (((c<<=1) & 0400)  !=  0)?1:0;
            crc ^= 0x1021;
        } else {
            crc <<= 1;
            crc += (((c<<=1) & 0400)  !=  0)?1:0;
        }
    }
}
*****************************************************/
