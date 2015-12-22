#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

void hexdump(const void* p, size_t length) {
	size_t i = 0;
	const int COLS = 16;
	const int ROWS = 16;
	while (i < length) {
		if (i % (COLS * ROWS) == 0) {
			cout << "     ";
			for (int col = 0; col < COLS; col++) {
				if (col == COLS / 2) {
					cout << " - ";
				}
				else {
					cout << " ";
				}
				cout << "+" << hex << setw(1) << col;
			}
			cout << endl;
			cout << "----: -----------------------   -----------------------" << endl;
		}
		for (int col = 0; col < COLS && i < length; col++) {
			unsigned char data = ((unsigned char*)p)[i++];
			if (col == 0) {
				cout << hex << setw(4) << setfill('0') << i << ":";
			}
			if (col == COLS / 2) {
				cout << " - ";
			}
			else {
				cout << " ";
			}
			cout << hex << setw(2) << setfill('0') << (unsigned short)data;
		}
		cout << endl;
		if (i % (COLS * ROWS) == 0 || i % COLS != 0) {
			cout << "----: -----------------------   ----------------------- " << endl;
			//cout << endl;
		}
	}
}
