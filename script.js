
/*--------------------------------------------------------------*/
/*  AES 128 Encryption/Decryption in JavaScript                 */
/*  ECE 5367 Team Discover                                      */
/*  Team Memebers:                                              */
/*  Charles Nguyen                                              */
/*  Kevin Nguyen                                                */
/*  Nhat Nguyen                                                 */
/*  Christopher Montealvo                                       */
/*  Thien Tao                                                   */         
/*--------------------------------------------------------------*/

/*
    This is a an implementation of AES (Rigndael cipher) encryption/decryption intended to 
    demonstrate the process of the algorithm. Implementation lacks optimization and should 
    be use when performance is not a priority. Both the encrypted data output and decrypted 
    input will be in hex. Padding is CMS(Cryptographic Message Syntax), this pads with the 
    same values as the number of padding bytes.

    Compare Answer with:
    https://www.devglan.com/online-tools/aes-encryption-decryption

    Resources:
    https://www.comparitech.com/blog/information-security/what-is-aes-encryption/
    https://zerofruit.medium.com/what-is-aes-step-by-step-fcb2ba41bb20
    https://kavaliro.com/wp-content/uploads/2014/03/AES.pdf
    https://braincoke.fr/blog/2020/08/the-aes-key-schedule-explained/#s-box
    https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
    https://8gwifi.org/CipherFunctions.jsp
    https://asecuritysite.com/encryption/padding


    This Website Link:
    https://teamdiscoveraes.netlify.app/

*/

//Import predefined tables from tables.js
import {sbox, inv_sbox, m2, m3, m9, m11, m13, m14, roundConstants} from './tables.js';

var state;
var keySchedule;

var isFile = 0;
var fileInput = document.getElementById('fileInput');
var fileData = [];

//Will run once user upload a file
fileInput.onchange = () =>{ 
   let file = fileInput.files[0];
   let reader = new FileReader();
   //Read content of file
   reader.readAsText(file);
   reader.onload = () =>{
       isFile = 1;
       //Store file content in fileData
       fileData = reader.result;
       console.log(fileData)
   };
   reader.onerror = () =>{
       //If any error, output to console for debugging
       console.log(reader.error);
   };
}


//Will run once user presses "Encrypt"
document.querySelector('#button1').addEventListener('click', function(){
    var keyInput = document.getElementById('key');
    var dataInput= document.getElementById('TextInput').value;
    console.log(dataInput);

    //Check if the plain text was from a file
    if(isFile){
        dataInput = fileData;
        isFile = 0;
    }
  

    state = [[],[],[],[]];
    keySchedule = [];


    if(keyInput.value.length != 16){
        alert('Length of Key need to be 16 for AES-128');
        return;
    }

    if(dataInput.length <=0){
        alert('Input data');
        return;
    }

    //Format the data. Split plain text into 16 bytes
    let formattedData = formatData(dataInput,"ascii");
    let encryptedData = "";

    //Beginning encrypting the data.
    //Loop will run once if length is less or equal to 16 bytes.
    for(let i = 0;i<formattedData.length;i++){
        encryptedData+= AES_encrypt(keyInput.value, formattedData[i]);
    }

    document.getElementById('subheader').style.display = 'none';
    document.getElementById('output').style.display = 'flex';
    document.getElementById('fileInput').value= null;
    //Output cyphertext back to user
    document.getElementById('output').innerHTML= encryptedData.toUpperCase();


});


document.querySelector('#button2').addEventListener('click', function(){
    var keyInput = document.getElementById('key');
    var dataInput = document.getElementById('TextInput').value;

    //Check if the plain text was from a file
    if(isFile){
        dataInput = fileData;
        isFile = 0;
    }
  
    state = [[],[],[],[]];
    keySchedule = [];


    if(keyInput.value.length != 16){
        alert('Length of Key need to be 16 for AES-128');
        return
    }

    if(dataInput.length <=0){
        alert('Input data');
        return;
    }

    let formattedData = formatData(dataInput,"hex");
    let decryptedData = "";

    //Begin decrypting cyphertext
    for(let i = 0;i<formattedData.length;i++){
        decryptedData+= AES_decrypt(keyInput.value, formattedData[i], i==formattedData.length-1?1:0);
    }

    document.getElementById('subheader').style.display = 'none';
    document.getElementById('output').style.display = 'flex';
    document.getElementById('fileInput').value= null;

    //output decoded data to user
    document.getElementById('output').innerHTML = decryptedData;



});



//Helper functions
let ASCItoHex = (x) => x.charCodeAt(0).toString(16)
let DecToASCI = (x) => String.fromCharCode(x)
let DecToHex = (x) => Number(x).toString(16)
let HexToDec = (x) => parseInt(x,16)

const printState = () =>{
    const temp = state.map(value => value.map(DecToHex));
    console.table(temp);
}
const printKeySchedule = () =>{
    const temp = keySchedule.map(value => value.map(DecToHex));
    console.table(temp);
}

//Format data into 16 bytes
const formatData = (data, dataType) =>{
    let arr = [];
    let maxLength = (dataType === "ascii")?16:32;
    for(let i=0, pos = 0;i<data.length/maxLength;i++){
        arr[i] = data.slice(pos,pos+maxLength);
        pos+=maxLength;
        if((i == data.length/maxLength - 1) && data.length%maxLength == 0 && dataType == "ascii"){
            arr[i+1] = "";
        }
    }
    return arr;
}

//AES encryption function
const AES_encrypt = (key, data) =>{
    //If KeySchedule has not been created yet, create a empty 4x44 key schedule
    if(!keySchedule.length){
        for(let i = 0, pos = 0; i<44;i++){
            keySchedule.push([0]);
            for(let j = 0; j<4;j++){
                keySchedule[i][j] = 0;
            }
        }
    }

    //Place plain text into a 4x4 array, implement padding if text length is less than 16
    for(let i = 0, pos = 0; i<4 ;i++){
        for(let j = 0; j<4; j++){
            if(pos < data.length){
                state[j][i] = HexToDec(ASCItoHex(data[pos]));
            }
            else{
                state[j][i] = 16 - data.length;
            }
            keySchedule[i][j] = HexToDec(ASCItoHex(key[pos]));
            pos++;
        }
    }

    //Fill in key schedule 
    for (let i = 1; i < 11; i++) {
		KeyExpansion(i);
	}

    AddRoundKeys(0);
    for (let i = 1; i < 10; i++) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKeys(i);
    }
	SubBytes();
	ShiftRows();
	AddRoundKeys(10);


    let output = "";
    for(let i=0;i<4;i++){
        for(let j =0;j<4;j++){
            let temp = DecToHex(state[j][i]);
            if(temp.length < 2){
                output+=0;
            }
            output+=DecToHex(state[j][i]);
        }
    }

    return output;
}

const AES_decrypt = (key, data, isLast) => {
    //If KeySchedule has not been created yet, create a empty 4x44 key schedule
    if(!keySchedule.length){
        for(let i = 0, pos = 0; i<44;i++){
            keySchedule.push([0]);
            for(let j = 0; j<4;j++){
                keySchedule[i][j] = 0;
            }
        }
    }

    for(let i = 0, pos = 0, pos2=0 ;i<4;i++){
        for(let j = 0; j<4;j++){
            state[j][i] = HexToDec(data[pos2] + data[pos2+1]);
            keySchedule[i][j] = HexToDec(ASCItoHex(key[pos]));
            pos++;
            pos2 = pos2+2;
        }
    }

    for (let i = 1; i < 11; i++) {
		KeyExpansion(i);
	}

    AddRoundKeys(10);
    InvShiftRows();
    InvSubBytes();

    for (let i = 9; i > 0; i--) {
        AddRoundKeys(i);
        InvMixColumns();
        InvShiftRows();
        InvSubBytes();
    }

    AddRoundKeys(0);

    if(isLast){
        for(let i = 3, counter = 0, temp = state[3][3];i>=0;i--){
            for(let j = 3;j>=0;j--){
                if(counter < temp){
                    state[j][i] = -1;
                    counter++;
                }
            }
        }
    }
    
    let output = "";
    for(let i=0;i<4;i++){
        for(let j =0;j<4;j++){
            if(!(isLast && state[j][i] === -1)){
                output+=DecToASCI(state[j][i]);
            }
        }
    }
    return output;
}
  

const SubBytes = () =>{
    for(let i = 0; i < state.length ; i++){
        for(let j = 0; j < state.length; j++){
            state[i][j] = sbox[state[i][j]];

        }
    }
}
const ShiftRows = () => {
    let temp = [[],[],[],[]];

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

	for (let i = 0; i < 4; i++) {
		for (let j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}
const MixColumns = () => {
    let temp = [[],[],[],[]];

	for (let i = 0; i < 4; i++) {
		temp[0][i] = (m2[state[0][i]] ^ m3[state[1][i]] ^ state[2][i] ^ state[3][i]);
		temp[1][i] = (state[0][i] ^ m2[state[1][i]] ^ m3[state[2][i]] ^ state[3][i]);
		temp[2][i] = (state[0][i] ^ state[1][i] ^ m2[state[2][i]] ^ m3[state[3][i]]);
		temp[3][i] = (m3[state[0][i]] ^ state[1][i] ^ state[2][i] ^ m2[state[3][i]]);

	}

	for (let i = 0; i < 4; i++) {
		for (let j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}
const KeyExpansion = (round) => {
    // generate the nth key round
	let row = round * 4;
	//printRow(schedule, row);
	// part 1: copy previous row into first row of this round
	for (let i = 0; i < 4; i++) {
		keySchedule[row][i] = keySchedule[row - 1][i];
	}

	// part 1: circular left shift
	let temp = keySchedule[row][0];
	for (let i = 0; i < 4 - 1; i++) {
		keySchedule[row][i] = keySchedule[row][i + 1];
	}
	keySchedule[row][3] = temp;

	// part 2: s box substitution
	for (let i = 0; i < 4; i++) {
		keySchedule[row][i] = sbox[keySchedule[row][i]];
	}
	// part 3: add round constant
	let rcon = roundConstants[round - 1]; // rcon is a special round constant computed by a complicated formula, the first 10 values are provided

	keySchedule[row][0] = keySchedule[row][0] ^ rcon;

	// part 3: perform a special g() xor with current row and n - 4
	for (let i = 0; i < 4; i++) {
		keySchedule[row][i] = keySchedule[row][i] ^ keySchedule[row - 4][i];
	}

	// part 4: next 3 rows are just n - 1 ^ n - 4
	for (let i = 1; i < 4; i++) {
		for (let j = 0; j < 4; j++) {
			keySchedule[row + i][j] = keySchedule[row + i - 1][j] ^ keySchedule[row + i - 4][j];
		}
	}
}

const AddRoundKeys = (round) =>{
	for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) {
            state[i][j] ^= keySchedule[j+(round*4)][i];
        }
	}
}

const InvSubBytes = () => {
    for(let i = 0; i < state.length ; i++){
        for(let j = 0; j < state.length; j++){
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}
const InvShiftRows = () => {
    let temp = [[],[],[],[]];

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

	for (let i = 0; i < 4; i++) {
		for (let j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}
const InvMixColumns = () => {
    let temp = [[],[],[],[]];

	for (let i = 0; i < 4; i++) {
		temp[0][i] = (m14[state[0][i]] ^ m11[state[1][i]] ^ m13[state[2][i]] ^ m9[state[3][i]]);
		temp[1][i] = (m9[state[0][i]] ^ m14[state[1][i]] ^ m11[state[2][i]] ^ m13[state[3][i]]);
		temp[2][i] = (m13[state[0][i]] ^ m9[state[1][i]] ^ m14[state[2][i]] ^ m11[state[3][i]]);
		temp[3][i] = (m11[state[0][i]] ^ m13[state[1][i]] ^ m9[state[2][i]] ^ m14[state[3][i]]);
	}

	for (let i = 0; i < 4; i++) {
		for (let j = 0; j < 4; j++) {
			state[i][j] = temp[i][j];
		}
	}
}


