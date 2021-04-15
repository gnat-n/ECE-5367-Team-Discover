// Nhat Nguyen
// ECE 5367
// 4/15/21
// Key Schedule Generation

#include <iostream>
#include <cstring>
#include <iomanip>

// resources:
// https://braincoke.fr/blog/2020/08/the-aes-key-schedule-explained/#s-box
// https://kavaliro.com/wp-content/uploads/2014/03/AES.pdf

using namespace std;

const unsigned char roundConstants[10] = {
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36
};

const unsigned char S_Box[16][16] = {
	{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
	{0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
	{0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
	{0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
	{0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
	{0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
	{0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
	{0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
	{0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
	{0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
	{0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
	{0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
	{0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
	{0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
	{0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
	{0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

void printRow(unsigned char key[][4], int row) {
	cout << "round Key for row " << row << endl;

	for (int j = 0; j < 4; j++) {
		cout << hex << (int)key[row][j] << " ";// << (int)(key[i][j] / 16) << "," << (int)(key[i][j] % 16) << " ";
	}
	cout << endl;
}

void printRoundKey(unsigned char key[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << hex << (int)key[i][j] << " ";// << (int)(key[i][j] / 16) << "," << (int)(key[i][j] % 16) << " ";
		}
		cout << endl;
	}
}

void printKeySchedule(unsigned char schedule[][4]) {
	for (int i = 0; i < 11; i++) {
		cout << "Round " << i << ": ";
		for (int n = 0; n < 4; n++) {
			for (int j = 0; j < 4; j++) {
				cout << hex << (int)schedule[i * 4 + n][j] << " ";// << (int)(key[i][j] / 16) << "," << (int)(key[i][j] % 16) << " ";
			}
		}
		cout << endl;
	}
}

void sSub(unsigned char schedule[][4], int row) {
	//performs element-wise s box substitution for a specific row

	for (int i = 0; i < 4; i++) {
		schedule[row][i] = S_Box[(int)(schedule[row][i] / 16)][(int)(schedule[row][i] % 16)];
	}
}

void gOperation(unsigned char schedule[][4], int row, int round) {
	// part 1: circular left shift
	unsigned char temp;
	temp = schedule[row][0];
	for (int i = 0; i < 4 - 1; i++) {
		schedule[row][i] = schedule[row][i + 1];
	}
	schedule[row][3] = temp;

	// part 2: s box substitution
	sSub(schedule, row);

	// part 3: add round constant
	int rcon = roundConstants[round-1]; // rcon is a special round constant computed by a complicated formula, the first 10 values are provided

	schedule[row][0] = schedule[row][0] ^ rcon;
}

void scheduleXOR(unsigned char schedule[][4], int row1, int row2, int destinationRow) {
	// performs element-wise XOR for row1 and row2, then places results in destination row

	for (int i = 0; i < 4; i++) {
		schedule[destinationRow][i] = schedule[row1][i] ^ schedule[row2][i];
	}
}

void generateRoundKey(unsigned char schedule[][4], int round) {
	// generate the nth key round
	int row = round * 4;

	//printRow(schedule, row);

	// part 1: copy previous row into first row of this round
	for (int i = 0; i < 4; i++) {
		schedule[row][i] = schedule[row-1][i];
	}

	//printRow(schedule, row);

	// part 2: perform the g() function on the row
	gOperation(schedule, row, round);

	// part 3: perform a special g() xor with current row and n - 4
	for (int i = 0; i < 4; i++) {
		schedule[row][i] = schedule[row][i] ^ schedule[row - 4][i];
	}

	//printRow(schedule, row);

	// part 4: next 3 rows are just n - 1 ^ n - 4
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			//cout << "accessing " << row + i << ", " << row + i - 1 << ", " << row + i - 4 << endl;
			schedule[row + i][j] = schedule[row + i - 1][j] ^ schedule[row + i - 4][j];
		}
		//printRow(schedule, row + i);
	}
}

int main() {
	// keyPlaintext gets passed into the text2bytebox function written by person 1

	unsigned char key[4][4] = {
		0x54,0x68,0x61,0x74,
		0x73,0x20,0x6D,0x79,
		0x20,0x4B,0x75,0x6E,
		0x67,0x20,0x46,0x75
	};

	unsigned char keySchedule[44][4]; // size is 4 + 4 * rounds, rounds is 10, so size is 44x4
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			keySchedule[i][j] = key[i][j];
		}
	}
	for (int i = 4; i < 11; i++) {
		for (int j = 0; j < 4; j++) {
			keySchedule[i][j] = 0;
		}
	}

	for (int i = 1; i < 11; i++) {
		generateRoundKey(keySchedule, i);
	}

	printKeySchedule(keySchedule);
}