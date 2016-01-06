class CRC16CCITT {
public:
    CRC16CCITT() : crc(0) { } 
    ~CRC16CCITT() { } 
    void update(char b) { calc_crc((int)b & 0xff); }
    void reset() { crc = 0; }
    int highByte() const { return (( crc >> 8) & 0xff); }
    int lowByte() const { return crc & 0xff; }
    int value() const { return crc; }
private:	
	int crc;
	void calc_crc(int c);
};
