#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

using namespace std;

void convertStringToBox(int boxNumber, string sTemp, int iCount )
{
	static char *sTempString;

	printf("convertStringToBox starts");
	for (int i = 0; i < boxNumber; i++)
	{
		static string* sBox =new string[4];
		for (int column = 0; column < 4; column++)
		{
			for (int row = 0; row < 4; row++)
			{
				//if (sTemp[iCount] == NULL) {}
				sBox[row][column] = sTempString[iCount];
				//if(sBox[row][column]==NULL){sBox[row][column]='00000000'}
			}
		}
	}
	printf("convertStringToBox ends");
}


void printBox(string **sBox)
{
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			cout << sBox[row][column] << "  ";
		}
		cout << endl;
	}
}

//User input flag and string
int main() {
	int iTemp, flag, boxNumber;
	int column = 0,row = 0, iCount=0, iTotalChar=0;
	string boxString[4][4];
	string binaryString;
	string myString;

	//  User input flag for encrytion/decryption
	cout << "Please enter 0 for encryption or 1 for decyption: ";
	cin >> flag; 
	cin.clear(); cin.ignore();

	if (!flag)  //Encryption
	{
		//  User input string for encyption or binary string for decrytion
		cout << "Please input string: ";
		getline(cin, myString);
		cout << "myString=" << myString << endl;

		//Calculate Number of boxNumber
		iTemp = myString.length();
		boxNumber = iTemp / 16 + (iTemp % 16 != 0);

		string* sTemp =new string[boxNumber*16];

		for (char& _char : myString) {
			binaryString = bitset<8>(_char).to_string();
			sTemp[iTotalChar] = binaryString;
			cout << "sTemp[" << iTotalChar << "]=" << sTemp[iTotalChar] << endl;
			iTotalChar++;
		}

		printf("convertStringToBox starts \n");
		for (int i = 0; i < boxNumber; i++)
		{
			//Dynamically allocate 2D array
			string** sBox = new string*[4];
			for (int i = 0; i < 4; i++) { sBox[i] = new string[4]; }
			
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
		}
		printf("convertStringToBox ends \n");

	}
	else //Decryption
	{
		cout << "Decryption starts here" << endl;
	}


	printf("\n *** END OF PROGRAM ***\n");
	getchar();
	getchar();
	return 0;
}