#include <stdio.h>
#include <string.h>

int CharBits[256];
int Blocks = 1;
int oldCipherBlocks[4][64];
int CipherBlocks[4][64];
int oldFPText[4][64];
int FPText[4][64];
int Key64[64];      // After checking Validity of String Key
int Key56[56];      // after removign Parity bit
// Round Key gen
int KeyL[28];
int KeyR[28];

int Key48[48]; // To be stored to RoudnKeys

// For 
int RoundKeys[16][48];

// For Round Function
int LRoundBlock[32];
int RRoundBlock[32];
int ExpDBoxOutputBlock[48];  // Output of Expansion DBox for input of Sboxes
int SBoxOutputBlock[32];     // Output of sbox for input of Straight Dbox
int StrDBoxOutputBlock[32];    // Output of Straight Dbox for input for XOring with LRoundBlock
int RTempBlock[32]; // For Xoring
int finalCharBits[256];

static int IPTable[64] =      // Initial Permutation Table
{   
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7   
};

static int FPTable[64] =      // Final Permutation Table
{   
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

static int ExpDBoxTable[] =    // Expantion Permutation Table
{
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static int StrDBoxTable[] =    // Straight Permutation Table
{
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

static int Sboxes[8][4][16] =  // SBox Permutation Tables
{
    {{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
    {0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
    {4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
    {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}},    //SBox 1

    {{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
    {3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
    {0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
    {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}},    //SBox 2

    {{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
    {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
    {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
    {1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}},     //SBox 3

    {{7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
    {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
    {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
    {3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}},     //SBox 4

    {{2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
    {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
    {4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
    {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}},    //SBox 5

    {{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
    {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
    {9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
    {4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}},     //SBox 6

    {{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
    {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
    {1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
    {6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}},     //SBox 7

    {{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
    {1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
    {7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
    {2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}}      //SBox 8


};

static int KeyParityDropTable[] =                                                 // Parity Drop Table 
{
    57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

static int KeyCompressionTable[] =                                                 // Compression D-box Table
{
    14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static int KeyShiftTable[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };  // Shift Table for Key Gen


int Swapper(){    // Swapping L Block and R Block
    for(int i = 0; i<32; i++){
        RTempBlock[i] = RRoundBlock[i];
        RRoundBlock[i] = LRoundBlock[i];
        LRoundBlock[i] = RTempBlock[i];
    }
    return 0;
}

int arrayXOR(int arrayA[], int arrayB[], int length){
    for (int i = 0; i<length; i++){
        arrayA[i] = (arrayA[i] ^ arrayB[i]);
    }
    return 0;
}

int SBoxPermutation(int i){
    // Get row and column decimal values from binary digits in array
    int row = ExpDBoxOutputBlock[i*6]*2 + ExpDBoxOutputBlock[i*6 + 5];
    int column = ExpDBoxOutputBlock[i*6 + 1]*8 + ExpDBoxOutputBlock[i*6 + 2]*4 + ExpDBoxOutputBlock[i*6+3]*2 + ExpDBoxOutputBlock[i*6 + 4];

    // Get the SBox permutation value based on row and column values
    int SBoxOutput = Sboxes[i][row][column];

    // Convert SBoxOutput to binary
    int dec = SBoxOutput;
    int array[4]={0,0,0,0};

    for(int i = 3; dec>0; i--){
        array[i] = dec % 2;
        dec = dec/2;
    }

    // Store the 4-digit binary in a 32-element array
    for (int x = i*4; x < (i+1)*4; x++){
        SBoxOutputBlock[x] = array[x%4];
    }
    return 0;
    
}

// Split ExpDBoxOutputBlock into 8 parts, 1 for each S-box
int SBoxSplit(){
    for(int i=0; i < 8; i++){
        SBoxPermutation(i);
    }
    return 0;
}

// Expand the 32-bit RRound into 48-bits using ExpDBoxtable
int ExpansionDBox(){
    for(int i = 0; i < 48; i++){
        ExpDBoxOutputBlock[i] = RRoundBlock[ExpDBoxTable[i]-1];
    }
    return 0;
}

// Straight permutation from SBoxOutputBlock using StrDBoxTable
int StraightDBox(){
    for(int i = 0; i < 32; i++){
        StrDBoxOutputBlock[i] = SBoxOutputBlock[StrDBoxTable[i]-1];
    }
    return 0;
}

// Initial permutation of 64-bits
int initialPermutation(int input[], int block){
    for(int i=0; i<64; i++){
        CipherBlocks[block][i] = input[IPTable[i]-1];
    }
    for(int j=0; j<32;j++){
        RRoundBlock[j]=CipherBlocks[block][j+32];
        LRoundBlock[j]=CipherBlocks[block][j];
    }
    return 0;
}

// Final permutation of 64-bits
int finalPermutation(int block){
    for(int i=0; i<64; i++){
        FPText[block][i] = oldFPText[block][FPTable[i]-1];
    }
    return 0;
}

// Function calls in a round
int roundFunction(int round, int block){
    ExpansionDBox();    // Expand R
    
    arrayXOR(ExpDBoxOutputBlock, RoundKeys[round], 48);     // Key XORing
    
    SBoxSplit();        // Split XORed ExpDBoxOutputBlock into 8 SBoxes

    StraightDBox();     // Straight Permutation after SBox Permutation
    
    arrayXOR(LRoundBlock, StrDBoxOutputBlock, 32);          // XOR StrDBox Output with LRoundBlock
    if(round!=15){
        Swapper();
    }
    return 0;
}


void des(int input[], int block){
    initialPermutation(input, block);
    for(int i=0; i<16; i++){
        roundFunction(i, block);
    }
    for(int j=0; j<32; j++){                    // Combine L and R to FPText
        oldFPText[block][j]=LRoundBlock[j];
        oldFPText[block][j+32]=RRoundBlock[j];
    }

    finalPermutation(block);                    // Permute to final permutation
}

int KeyCheck(char key[]){
    int sum=0;
    if(strlen(key)!=64){
        return -1;
    }
    for(int i=0; i<64; i++){
        sum += key[i]-48;
        Key64[i] = (int)key[i]-48;
        if((i+1)%8 == 0){
            if(sum%2==0){
                return -1;
            }
            sum = 0;
        }
    }
    return 0;
}
int shiftLeft(int RoundKeyBlock[], int numOfShifts){
    for(int i = 0; i< numOfShifts; i++){
        int temp = RoundKeyBlock[0];
        for(int j = 1; j < 28; j++){
            RoundKeyBlock[j-1] = RoundKeyBlock[j];
        }
        RoundKeyBlock[27] = temp;
    }
    return 0;
}


int KeyParityDrop(){
    for(int i = 0; i < 56; i++){
        Key56[i] = Key64[KeyParityDropTable[i]-1];
    }
    return 0;
}

int KeySplit(){
    for(int i=0; i<28; i++){
        KeyL[i] = Key56[i];
        KeyR[i] = Key56[i+28];
    }
    return 0;
}

int KeyCompress(int round){
    int newKey56[56];
    for(int j = 0; j<56; j++){
        if(j<28){
            newKey56[j] = KeyL[j];
        }
        else{
            newKey56[j] = KeyR[j-28];
        }
    }

    for(int i=0; i<48; i++){
        RoundKeys[round][i] = newKey56[KeyCompressionTable[i]-1];
    }
    return 0;
}
int KeyGen(){
    KeyParityDrop();        // Parity Drop
    KeySplit();
    
    
    for(int i = 0; i<16; i++){
        shiftLeft(KeyL, KeyShiftTable[i]);
        shiftLeft(KeyR, KeyShiftTable[i]);
        KeyCompress(i);
    }
    
    return 0;
}

int *DectoBin(int dec){
    static int array[8]={0,0,0,0,0,0,0,0};
    for(int j=0; j<8; j++){
        array[j]=0;
    }
    for(int i = 7; dec>0; i--){
        array[i] = dec % 2;
        dec = dec/2;
    }
    return array;
}

int minInt(int a, int b){
    if (a>b){
        return b;
    }
    else {
        return a;
    }
}

void StringConvertToBinary(char input[]){

    if(strlen(input)>8 && strlen(input)<16){
        Blocks = 2;
    }
    else if(strlen(input)>24){
        Blocks = 4;
    }
    else if(strlen(input)>16){
        Blocks = 3;
    }
    for(int i=0; i<strlen(input); i++){
        int *BinArray = DectoBin((int)input[i]); 
        for(int j=0; j<8; j++){
            CharBits[i*8 + j] = BinArray[j];
        }
    }
    
}

int main(){
    char input[33];
    char key[65];
    for(int a=0; a<256; a++){   // Initialize CharBits to all be 0
        CharBits[a] = 0;
        finalCharBits[a] = 0;
    }

    scanf("%[^\n]", input);
    getchar();                      // get newline
    scanf("%[^\n]", key);
    getchar();                      // get newline

    if(strlen(input) > 32)
    {
        input[32] = '\0';
    }
   
    if(KeyCheck(key) == 0){                 // Check Key validity
        KeyGen();

    // TODO:
    // PLAINTEXT PROCESSING
        StringConvertToBinary(input);               // Convert 
        for(int i=0; i<64; i++){                    // Split into 64-bit blocks for DES Input
            oldCipherBlocks[0][i] = CharBits[i];
            oldCipherBlocks[1][i] = CharBits[i+64];
            oldCipherBlocks[2][i] = CharBits[i+128];
            oldCipherBlocks[3][i] = CharBits[i+192];
        }
        for(int j=0; j<Blocks; j++){
            des(oldCipherBlocks[j],j);
            for(int n=0; n<64; n++ ){
                finalCharBits[n+j*64]=FPText[j][n];
            }
        }
    }
    else{
        printf("invalid key");
        return 0;
    }
    for(int z=0; z<256; z++){
        if(z<(Blocks)*64){
            printf("%d",finalCharBits[z]);
        }
    }

    return 0;
}