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
void InvSubBytes_helper(unsigned char s_box[][16],unsigned char need, int &r, int &c);
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
	unsigned char s_box[16][16] = 
  	{
	    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
	    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
	    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
	    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
	    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
	    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
	    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
	    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
	    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
	    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
	    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
	    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
	    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
	    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
	    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
	    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16},
	};
	int MSBs = 0;
  	int LSBs = 0;

	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			MSBs = (state[i][j] & 0xf0);
			MSBs >>= 4;
			LSBs = (state[i][j] & 0x0f);
			state[i][j] = s_box[MSBs][LSBs];
		}
	}
}
void ShiftRows(unsigned char state[][4]) {
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
	[s0] [s4] [s8]	[s12]		[2][3][1][1]		
	[s1] [s5] [s9]	[s13]		[1][2][3][1]		
	[s2] [s6] [s10] [s14]		[1][1][2][3]		
	[s3] [s7] [s11] [s15]		[3][1][1][2]		

	[2]	[3]	[1]	[1]	[s0]	[s0']
	[1]	[2]	[3]	[1]  *	[s1] =	[s1']
	[1]	[1]	[2]	[3]	[s2]	[s2']
	[3]	[1]	[1]	[2]	[s3]	[s3']

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
	unsigned char s_box[16][16] = 
  	{
	    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
	    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
	    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
	    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
	    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
	    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
	    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
	    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
	    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
	    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
	    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
	    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
	    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
	    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
	    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
	    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16},
	};
	
	int MSBs;
	int LSBs;
	int replace = 0x00;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
		      MSBs = 0;
		      LSBs = 0;
		      replace = 0x00;

		      byte_sub_helper(s_box, state[i][j], MSBs, LSBs);

		      MSBs <<= 4;
		      replace = (replace | MSBs);
		      replace = (replace | LSBs);

		      state[i][j] = replace;
	    	}
	}
}

void InvSubBytes_helper(unsigned char s_box[][16], unsigned char need, int &r, int &c){
	for(int i = 0; i < 16; i++){
    		for(int j = 0; j < 16; j++){
      			if(need == s_box[i][j]){
        			r = i;
        			c = j;
        			return;
      			}
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
	[9][14][11][13]  *	[s1] =	[s1']
	[13][9][14][11]		[s2]	[s2']
	[11][13][9][14]		[s3]	[s3']

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

