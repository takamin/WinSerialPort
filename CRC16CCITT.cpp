#include <stdint.h>
#include "C99_types.h"
#include "CRC16CCITT.h"

uint16_t CRC16CCITT::INITIAL_VALUE = 0xffff;

/*
 * http://web.mit.edu/6.115/www/miscfiles/amulet/amulet-help/xmodem.htm
 */
void CRC16CCITT::calc_crc(uint8_t c) {
    int count;
    crc ^= (uint16_t)(c << 8);
    for (count=8; --count>=0;) {
        if ((crc & (uint16_t)0x8000) != 0) {
            crc <<= 1;
            crc ^= (uint16_t)0x1021;
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
