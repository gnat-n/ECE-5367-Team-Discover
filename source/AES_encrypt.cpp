#include <iostream>
#include "tables.h"
#include <sstream>
#include <bitset>

using namespace::std;

void printCheck(unsigned char state[][4]);
void KeyExpansion(unsigned char keySchedule[44][4], int round);
void SubBytes(unsigned char state[][4]);
void ShiftRows(unsigned char state[][4]);
void MixColumns(unsigned char state[][4]);
void AddRoundKeys(unsigned char state[][4], unsigned char key[][4], int round);

void encrypt(unsigned char input[17], unsigned char key[][4]);
void decrypt(unsigned char input[17], unsigned char key[][4]);

void InvSubBytes(unsigned char state[][4]);
void InvShiftRows(unsigned char state[][4]);
void InvMixColumns(unsigned char state[][4]);

int main(){

  unsigned char temp[17] ="Two One Nine Two";
  
	unsigned char key[4][4] = {
		0x54,0x68,0x61,0x74,
		0x73,0x20,0x6D,0x79,
		0x20,0x4B,0x75,0x6E,
		0x67,0x20,0x46,0x75
	};

  /*unsigned char temp[17] ={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

  unsigned char key[4][4] = {
    0x00,0x01,0x02,0x03,
    0x04,0x05,0x06,0x07,
    0x08,0x09,0x0a,0x0b,
    0x0c,0x0d,0x0e,0x0f
  };
  */
  /*unsigned char key[4][4] = {
    0x00,0x04,0x08,0x0c,
    0x01,0x05,0x09,0x0d,
    0x02,0x06,0x0a,0x0e,
    0x03,0x07,0x0b,0x0f
  };*/
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
	/*unsigned char test[4][4] = {
		0x63,0xeb,0x9f,0xa0,
		0x2f,0x93,0x92,0xc0,
		0xaf,0xc7,0xab,0x30,
		0xa2,0x20,0xcb,0x2b
	};*/

	//After mix columns
	/*unsigned char test[4][4] = {
		0xba,0x84,0xe8,0x1b,
		0x75,0xa4,0x8d,0x40,
		0xf4,0x8d,0x06,0x7d,
		0x7a,0x32,0x0e,0x5d
	};*/

	encrypt(temp, key);
  decrypt(temp, key);
	return 0;
}
	

void printKeySchedule(unsigned char schedule[][4]) {
	for (int i = 0; i < 11; i++) {
		std::cout << "Round " << i << ": ";
		for (int n = 0; n < 4; n++) {
			for (int j = 0; j < 4; j++) {
				cout << hex << (int)schedule[i * 4 + n][j] << " ";
			}
		}
		cout << endl;
	}
}

void encrypt(unsigned char input[17], unsigned char key[][4]) {
	// size is 4 + 4 * rounds, rounds is 10, so size is 44x4
	unsigned char keySchedule[44][4] = { 0 };
	unsigned char state[4][4];
	int pos = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = input[pos];
			keySchedule[i][j] = key[i][j];
			pos++;
		}
	}

	for (int i = 1; i < 11; i++) {
		KeyExpansion(keySchedule, i);
	}

  // round 0
	AddRoundKeys(state, keySchedule, 0);

	for (int i = 1; i < 10; i++) {
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
    // round 1 - 9
		AddRoundKeys(state, keySchedule, i);
	}
	SubBytes(state);
	ShiftRows(state);
  // round a
	AddRoundKeys(state, keySchedule, 10);

  pos = 0;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      input[pos] = state[j][i];
      pos++;
    }
  }
  std::cout<<"Encrypted Data: ";
  for(int i=0;i<16;i++){
    std::cout<<hex<<(int)input[i]<<" ";
  }
  cout << endl;
}

void decrypt(unsigned char input[16], unsigned char key[][4]) {
	// size is 4 + 4 * rounds, rounds is 10, so size is 44x4
	unsigned char keySchedule[44][4] = { 0 };
	unsigned char state[4][4];
	int pos = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = input[pos];
			keySchedule[i][j] = key[i][j];
			pos++;
		}
	}

	for (int i = 1; i < 11; i++) {
		KeyExpansion(keySchedule, i);
	}

  // round 0
	AddRoundKeys(state, keySchedule, 10);

  InvShiftRows(state);
  std::cout<<"Shift Rows:"<<std::endl;
  printCheck(state);

  InvSubBytes(state);
  std::cout<<"Sub Bytes:"<<std::endl;
  printCheck(state);

	for (int i = 9; i > 0; i--) {
    // round 1 - 9
		AddRoundKeys(state, keySchedule, i);
    
    InvMixColumns(state);
    std::cout<<"Mix Columns:"<<std::endl;
    printCheck(state);

    InvShiftRows(state);
    std::cout<<"Shift Rows:"<<std::endl;
    printCheck(state);

    InvSubBytes(state);
    std::cout<<"Sub Bytes:"<<std::endl;
    printCheck(state);
    
	}
  // round a
	AddRoundKeys(state, keySchedule, 0);

  pos = 0;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      input[pos] = state[j][i];
      pos++;
    }
  }
  std::cout<<"Decrypted Data: ";
  for(int i=0;i<16;i++){
    std::cout<<hex<<(int)input[i]<<" ";
  }
  cout << endl;
}


void KeyExpansion(unsigned char schedule[][4], int round) {
	// generate the nth key round
	int row = round * 4;
	//printRow(schedule, row);

	// part 1: copy previous row into first row of this round
	for (int i = 0; i < 4; i++) {
		schedule[row][i] = schedule[row - 1][i];
	}

	// part 1: circular left shift
	unsigned char temp;
	temp = schedule[row][0];
	for (int i = 0; i < 4 - 1; i++) {
		schedule[row][i] = schedule[row][i + 1];
	}
	schedule[row][3] = temp;

	// part 2: s box substitution
	for (int i = 0; i < 4; i++) {
		schedule[row][i] = sbox2[(int)(schedule[row][i] / 16)][(int)(schedule[row][i] % 16)];
	}
	// part 3: add round constant
	int rcon = roundConstants[round - 1]; // rcon is a special round constant computed by a complicated formula, the first 10 values are provided

	schedule[row][0] = schedule[row][0] ^ rcon;

	// part 3: perform a special g() xor with current row and n - 4
	for (int i = 0; i < 4; i++) {
		schedule[row][i] = schedule[row][i] ^ schedule[row - 4][i];
	}

	// part 4: next 3 rows are just n - 1 ^ n - 4
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			schedule[row + i][j] = schedule[row + i - 1][j] ^ schedule[row + i - 4][j];
		}
	}
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
	for (int i = 0; i < 4; i++) {
		temp[0][i] = (unsigned char)(m2[state[0][i]] ^ m3[state[1][i]] ^ state[2][i] ^ state[3][i]);
		temp[1][i] = (unsigned char)(state[0][i] ^ m2[state[1][i]] ^ m3[state[2][i]] ^ state[3][i]);
		temp[2][i] = (unsigned char)(state[0][i] ^ state[1][i] ^ m2[state[2][i]] ^ m3[state[3][i]]);
		temp[3][i] = (unsigned char)(m3[state[0][i]] ^ state[1][i] ^ state[2][i] ^ m2[state[3][i]]);

	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}

void AddRoundKeys(unsigned char state[][4], unsigned char key[][4], int round) {
  std::cout<<"Round: " << round<<std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
      std::cout<<hex << (int) key[i+(round*4)][j]<<" ";
			state[i][j] ^= (unsigned char)key[j+(round*4)][i];
		}
	}
  cout << endl;
  cout<<"New key: "<<endl;
  printCheck(key);
  cout<<"Current State: "<<endl;

  printCheck(state);
  std::cout<<std::endl;
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
	for (int i = 0; i < 4; i++) {
		temp[0][i] = (unsigned char)(m14[state[0][i]] ^ m11[state[1][i]] ^ m13[state[2][i]] ^ m9[state[3][i]]);
		temp[1][i] = (unsigned char)(m9[state[0][i]] ^ m14[state[1][i]] ^ m11[state[2][i]] ^ m13[state[3][i]]);
		temp[2][i] = (unsigned char)(m13[state[0][i]] ^ m9[state[1][i]] ^ m14[state[2][i]] ^ m11[state[3][i]]);
		temp[3][i] = (unsigned char)(m11[state[0][i]] ^ m13[state[1][i]] ^ m9[state[2][i]] ^ m14[state[3][i]]);
	}

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
			std::cout << std::hex << (int)state[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

