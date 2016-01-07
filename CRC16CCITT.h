#include <stdint.h>
#include "C99_types.h"

class CRC16CCITT {
public:
    static uint16_t INITIAL_VALUE;
public:
    CRC16CCITT() : crc(INITIAL_VALUE) { }
    CRC16CCITT(uint16_t init_value) : crc(init_value) { }
    ~CRC16CCITT() { }
    void update(uint8_t bytedata) { calc_crc(bytedata & 0xff); }
    void reset() { crc = 0; }
    uint8_t highByte() const { return (( crc >> 8) & 0xff); }
    uint8_t lowByte() const { return crc & 0xff; }
    uint16_t value() const { return crc; }
private:
	uint16_t crc;
	void calc_crc(uint8_t c);
};
