#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

using namespace std;

void printBox(string **sBox)
{
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			cout << sBox[row][column] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//User input flag and string
int main() {
	int iTemp, boxNumber;
	int column = 0,row = 0, iCount, iCount2, iTotalChar=0;
	string boxString[4][4];
	string binaryString, binaryString2;
	string myString, myString2="";

	//  User input string for encyption or binary string for decrytion
	cout << "Please input string: ";
	getline(cin, myString);

	//Calculate Number of boxNumber
	iTemp = myString.length();
	boxNumber = iTemp / 16 + (iTemp % 16 != 0);

	string* sTemp = new string[boxNumber * 16];
	string* sTemp2 =new string[boxNumber * 16];

	for (char& _char : myString) {
		binaryString = bitset<8>(_char).to_string();
		sTemp[iTotalChar] = binaryString;
		//cout << "sTemp[" << iTotalChar << "]=" << sTemp[iTotalChar] << endl;
		iTotalChar++;
	}

	iCount = 0; iCount2 = 0;
	for (int i = 0; i < boxNumber; i++)
	{
		//Dynamically allocate 2D array
		string** sBox = new string*[4];
		for (int i = 0; i < 4; i++) { sBox[i] = new string[4]; }
			
		//Covert binary string to box[4x4]
		for (column = 0; column < 4; column++)
		{
			for (row = 0; row < 4; row++)
			{
				if (sTemp[iCount] == "") { sTemp[iCount] = "00000000"; }
				sBox[row][column] = sTemp[iCount];
				//cout << "sBox[" << row << "]" << "[" << column << "]=" << sBox[row][column] << endl;
				iCount++;
			}
		}
		printBox(sBox);

		//Covert binary box[4x4] string to binary string
		for (column = 0; column < 4; column++)
		{
			for (row = 0; row < 4; row++)
			{
				sTemp2[iCount2] = sBox[row][column];
				//cout << "sBox[" << row << "]" << "[" << column << "]=" << sBox[row][column] << endl;
				//cout << "sTemp2[" << iCount2 << "]=" << sTemp2[iCount2] << endl; 
				iCount2++;
			}
		}
			
	}
	for (int i = 0; i < (boxNumber*16); i++)
	{
		myString2 += bitset<8>(sTemp2[i]).to_ulong();
	}
	cout << "Result binary to string: " << myString2 << endl;

	printf("\n *** END OF PROGRAM ***\n");
	getchar();
	getchar();
	return 0;
}