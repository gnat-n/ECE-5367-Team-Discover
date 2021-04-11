#include <iostream>
#include "tables.h"

void printCheck(unsigned char state[][4]);
void KeyExpansion();
void SubBytes(unsigned char state[][4]);
void ShiftRows(unsigned char state[][4]);
void MixColumns(unsigned char state[][4]);
void AddRoundKeys(unsigned char state[][4], char* key);
void encrypt(unsigned char* input, char* key);

void InvSubBytes(unsigned char state[][4]);
void InvShiftRows(unsigned char state[][4]);
void InvMixColumns(unsigned char state[][4]);

int main() {
	unsigned char temp[16] = "TestingTestin12";
	char key[16] = "TestingTestin12";
	encrypt(temp,key);

	/*unsigned char test[4][4] = {
		0x00,0x3c,0x6e,0x47,
		0x1f,0x4e,0x22,0x74,
		0x0e,0x08,0x1b,0x31,
		0x54,0x59,0x0b,0x1a
	};*/

	//After sub byte
	//unsigned char test[4][4] = {
	//	0x63,0xeb,0x9f,0xa0,
	//	0xc0,0x2f,0x93,0x92,
	//	0xab,0x30,0xaf,0xc7,
	//	0x20,0xcb,0x2b,0xa2
	//};

	//After shift rows
	//unsigned char test[4][4] = {
	//	0x63,0xeb,0x9f,0xa0,
	//	0x2f,0x93,0x92,0xc0,
	//	0xaf,0xc7,0xab,0x30,
	//	0xa2,0x20,0xcb,0x2b
	//};

	//After mix columns
	unsigned char test[4][4] = {
		0xba,0x84,0xe8,0x1b,
		0x75,0xa4,0x8d,0x40,
		0xf4,0x8d,0x06,0x7d,
		0x7a,0x32,0x0e,0x5d
	};
	InvMixColumns(test);
	printCheck(test);

	return 0;
}
void encrypt(unsigned char* input, char* key) {
	unsigned char state[4][4];
	int pos = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = input[pos];
			pos++;
		}
	}
	SubBytes(state);
	ShiftRows(state);
	/*AddRoundKeys(state, key);
	for (int i = 0; i < 9; i++) {
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKeys(state, key);
	}
	SubBytes(state);
	ShiftRows(state);
	AddRoundKeys(state, key);*/
}


void KeyExpansion() {

}
void SubBytes(unsigned char state[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = sbox[state[i][j]];
		}
	}
}
void ShiftRows(unsigned char state[][4]) {
	/*	(Index, not values)
		[0]	[4]	[8]	 [12]
		[1]	[5]	[9]	 [13]
		[2]	[6]	[10] [14]
		[3]	[7]	[11] [15]
	to
		[0]	 [4]  [8]  [12]
		[5]	 [9]  [13] [1]
		[10] [14] [2]  [6]
		[15] [3]  [7]  [11]
	*/
	unsigned char temp[4][4];
	//1st row (No change)
	temp[0][0] = state[0][0];
	temp[0][1] = state[0][1];
	temp[0][2] = state[0][2];
	temp[0][3] = state[0][3];

	//2nd row (Left shift 1 step)
	temp[1][0] = state[1][1];
	temp[1][1] = state[1][2];
	temp[1][2] = state[1][3];
	temp[1][3] = state[1][0];

	//3rd row (Left shift 2 step)
	temp[2][0] = state[2][2];
	temp[2][1] = state[2][3];
	temp[2][2] = state[2][0];
	temp[2][3] = state[2][1];

	//4th row (Right shift 1 step)
	temp[3][0] = state[3][3];
	temp[3][1] = state[3][0];
	temp[3][2] = state[3][1];
	temp[3][3] = state[3][2];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}
void MixColumns(unsigned char state[][4]) {
	/* (Values, not index)
	[s0] [s4] [s8]	[s12]		[2]	[3]	[1]	[1]		
	[s1] [s5] [s9]	[s13]		[1]	[2]	[3]	[1]		
	[s2] [s6] [s10] [s14]		[1]	[1]	[2]	[3]		
	[s3] [s7] [s11] [s15]		[3]	[1]	[1]	[2]		

	[2]	[3]	[1]	[1]		[s0]	[s0']
	[1]	[2]	[3]	[1]  *	[s1] =	[s1']
	[1]	[1]	[2]	[3]		[s2]	[s2']
	[3]	[1]	[1]	[2]		[s3]	[s3']

	Example: 
	[s0]
	[s1]  * [2][3][1][1] = [s0']
	[s2]
	[s3]
	*/

	unsigned char temp[4][4];
	temp[0][0] = (unsigned char)(m2[state[0][0]]^ m3[state[1][0]]^ state[2][0]^ state[3][0]);	//[2][3][1][1]	
	temp[1][0] = (unsigned char)(state[0][0]^ m2[state[1][0]]^ m3[state[2][0]] ^ state[3][0]);	//[1][2][3][1]	
	temp[2][0] = (unsigned char)(state[0][0]^ state[1][0]^ m2[state[2][0]]^ m3[state[3][0]]);	//[1][1][2][3]	
	temp[3][0] = (unsigned char)(m3[state[0][0]]^ state[1][0]^ state[2][0]^ m2[state[3][0]]);	//[3][1][1][2]

	temp[0][1] = (unsigned char)(m2[state[0][1]]^ m3[state[1][1]]^ state[2][1]^ state[3][1]);	//[2][3][1][1]	
	temp[1][1] = (unsigned char)(state[0][1]^ m2[state[1][1]]^ m3[state[2][1]]^ state[3][1]);	//[1][2][3][1]	
	temp[2][1] = (unsigned char)(state[0][1]^ state[1][1]	^ m2[state[2][1]]^ m3[state[3][1]]);	//[1][1][2][3]	
	temp[3][1] = (unsigned char)(m3[state[0][1]]^ state[1][1]^ state[2][1]^ m2[state[3][1]]);	//[3][1][1][2]

	temp[0][2] = (unsigned char)(m2[state[0][2]]^ m3[state[1][2]]^ state[2][2]^ state[3][2]);	//[2][3][1][1]	
	temp[1][2] = (unsigned char)(state[0][2]^ m2[state[1][2]]^ m3[state[2][2]]^ state[3][2]);	//[1][2][3][1]	
	temp[2][2] = (unsigned char)(state[0][2]^ state[1][2]	^ m2[state[2][2]]^ m3[state[3][2]]);	//[1][1][2][3]	
	temp[3][2] = (unsigned char)(m3[state[0][2]]^ state[1][2]^ state[2][2]	^ m2[state[3][2]]);	//[3][1][1][2]

	temp[0][3] = (unsigned char)(m2[state[0][3]]^ m3[state[1][3]]^ state[2][3]^ state[3][3]);	//[2][3][1][1]	
	temp[1][3] = (unsigned char)(state[0][3]^ m2[state[1][3]]^ m3[state[2][3]]^ state[3][3]);	//[1][2][3][1]	
	temp[2][3] = (unsigned char)(state[0][3]^ state[1][3]	^ m2[state[2][3]]^ m3[state[3][3]]);	//[1][1][2][3]	
	temp[3][3] = (unsigned char)(m3[state[0][3]]^ state[1][3]^ state[2][3]	^ m2[state[3][3]]);	//[3][1][1][2]

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}
void AddRoundKeys(unsigned char state[][4], char* key) {

}

void InvSubBytes(unsigned char state[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = inv_sbox[state[i][j]];
		}
	}
}
void InvShiftRows(unsigned char state[][4]) {
	unsigned char temp[4][4];
	//1st row (No change)
	temp[0][0] = state[0][0];
	temp[0][1] = state[0][1];
	temp[0][2] = state[0][2];
	temp[0][3] = state[0][3];

	//2nd row (Right shift 1 step)
	temp[1][0] = state[1][3];
	temp[1][1] = state[1][0];
	temp[1][2] = state[1][1];
	temp[1][3] = state[1][2];

	//3rd row (Right shift 2 step)
	temp[2][0] = state[2][2];
	temp[2][1] = state[2][3];
	temp[2][2] = state[2][0];
	temp[2][3] = state[2][1];

	//4th row (Left shift 1 step)
	temp[3][0] = state[3][1];
	temp[3][1] = state[3][2];
	temp[3][2] = state[3][3];
	temp[3][3] = state[3][0];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}
void InvMixColumns(unsigned char state[][4]) {
	/* (Values, not index)
	[s0] [s4] [s8]	[s12]		[0E][0B][0D][09]
	[s1] [s5] [s9]	[s13]		[09][0E][0B][0D]
	[s2] [s6] [s10] [s14]		[0D][09][0E][0B]
	[s3] [s7] [s11] [s15]		[0B][0D][09][0E]

	[14][11][13][9]		[s0]	[s0']
	[9]	[14][11][13]  *	[s1] =	[s1']
	[13][9]	[14][11]	[s2]	[s2']
	[11][13][9]	[14]	[s3]	[s3']

	Example:
	[s0]
	[s1]  * [14][11][13][9] = [s0']
	[s2]
	[s3]
	*/

	unsigned char temp[4][4];
	temp[0][0] = (unsigned char)(m14[state[0][0]] ^ m11[state[1][0]] ^ m13[state[2][0]] ^ m9[state[3][0]]);	//[14][11][13][9]	
	temp[1][0] = (unsigned char)(m9[state[0][0]] ^ m14[state[1][0]] ^ m11[state[2][0]] ^ m13[state[3][0]]);	//[9][14][11][13]	
	temp[2][0] = (unsigned char)(m13[state[0][0]] ^ m9[state[1][0]] ^ m14[state[2][0]] ^ m11[state[3][0]]);	//[13][9][14][11]	
	temp[3][0] = (unsigned char)(m11[state[0][0]] ^ m13[state[1][0]] ^ m9[state[2][0]] ^ m14[state[3][0]]);	//[11][13][9][14]

	temp[0][1] = (unsigned char)(m14[state[0][1]] ^ m11[state[1][1]] ^ m13[state[2][1]] ^ m9[state[3][1]]);	//[14][11][13][9]	
	temp[1][1] = (unsigned char)(m9[state[0][1]] ^ m14[state[1][1]] ^ m11[state[2][1]] ^ m13[state[3][1]]);	//[9][14][11][13]
	temp[2][1] = (unsigned char)(m13[state[0][1]] ^ m9[state[1][1]] ^ m14[state[2][1]] ^ m11[state[3][1]]);	//[13][9][14][11]
	temp[3][1] = (unsigned char)(m11[state[0][1]] ^ m13[state[1][1]] ^ m9[state[2][1]] ^ m14[state[3][1]]);	//[11][13][9][14]

	temp[0][2] = (unsigned char)(m14[state[0][2]] ^ m11[state[1][2]] ^ m13[state[2][2]] ^ m9[state[3][2]]);	//[14][11][13][9]		
	temp[1][2] = (unsigned char)(m9[state[0][2]] ^ m14[state[1][2]] ^ m11[state[2][2]] ^ m13[state[3][2]]);	//[9][14][11][13]
	temp[2][2] = (unsigned char)(m13[state[0][2]] ^ m9[state[1][2]] ^ m14[state[2][2]] ^ m11[state[3][2]]);	//[13][9][14][11]	
	temp[3][2] = (unsigned char)(m11[state[0][2]] ^ m13[state[1][2]] ^ m9[state[2][2]] ^ m14[state[3][2]]);	//[11][13][9][14]

	temp[0][3] = (unsigned char)(m14[state[0][3]] ^ m11[state[1][3]] ^ m13[state[2][3]] ^ m9[state[3][3]]);	//[14][11][13][9]	
	temp[1][3] = (unsigned char)(m9[state[0][3]] ^ m14[state[1][3]] ^ m11[state[2][3]] ^ m13[state[3][3]]);	//[9][14][11][13]
	temp[2][3] = (unsigned char)(m13[state[0][3]] ^ m9[state[1][3]] ^ m14[state[2][3]] ^ m11[state[3][3]]);	//[13][9][14][11]		
	temp[3][3] = (unsigned char)(m11[state[0][3]] ^ m13[state[1][3]] ^ m9[state[2][3]] ^ m14[state[3][3]]);	//[11][13][9][14]

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}



//Help Functions
void printCheck(unsigned char state[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << std::hex<<(int)state[i][j]<<"\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

