#include <stdio.h>
#include <string.h>


// split string into 2 lines
// Plaintext = first line
// Key = 2nd line

// PLAINTEXT PROCESSING
    // String to binary
    // Split plaintext binary to 64 bit blocks Block 1 and 2 if more than 8 characters

// KEY PROCESSING
    // Key Check
        // Split into 8 character arrays
        // change each character into integer
        // add all into a variable
        // if even, return 1
        // if odd, return 0

    // Key Generation (Generate 16 round keys)
        // Parity Drop
        // Compression
        // Shift Table 

// DES (Block 1 and 2)
    // DONE Initial Permutation
    //Round Function
        // Binary split into 32 bits R and L
        // R to DES Function
            // Expansion D-box
            // XOR with Round Key
            // Split to S-boxes
            // Straight Permutation D-box
        // Xor with L, store to R-temp
        // R stored to L, R-temp stored to R
        // Next rounds
    // DONE Final Permutation

int CipherBlocks[2][64];
int FPText[2][64];

// For Key Generation
int RoundKeys[16][64];

// For Round Function
int LRound[32];
int RRound[32];
int ExpDBoxOutput[48];
int StraightBlock[32];
int RTemp[32]; // For Xoring


int IPTable[] =      // Initial Permutation Table
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

int FPTable[] =      // Final Permutation Table
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

int ExpDBoxTable[] =
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

int Sboxes[8][4][16] = {
    {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},    //SBox 1

    {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},    //SBox 2

    {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},     //SBox 3

    {7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},     //SBox 4

    {2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},    //SBox 5

    {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},     //SBox 6

    {4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},     //SBox 7

    {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}      //SBox 8


};

int SBoxPermutation(int i){
    printf("\nSBox %d\n",i+1);
    for(int j=i*6; j<(i+1)*6; j++){
        printf("%d",ExpDBoxOutput[j]);
    }
    printf("\n");
    int row = ExpDBoxOutput[i*6]*2 + ExpDBoxOutput[i*6 + 5];
    int column = ExpDBoxOutput[i*6 + 1]*8 + ExpDBoxOutput[i*6 + 2]*4 + ExpDBoxOutput[i*6+3]*2 + ExpDBoxOutput[i*6 + 4];
    printf("Row: %d\nColumn: %d\n",row,column);

    int SBoxOutput = Sboxes[i][row][column];
    printf("SBoxOutput: %d\n",SBoxOutput);


    
}
int SBoxSplit(){
    for(int i=0; i < 8; i++){
        SBoxPermutation(i);
    }
}
int ExpansionDBox(){
    printf("\nExpansionD Box Output\n");
    for(int i = 0; i < 48; i++){
        ExpDBoxOutput[i] = RRound[ExpDBoxTable[i]-1];
        printf("%d",ExpDBoxOutput[i]);
    }
    printf("\n");
}

int initialPermutation(char input[]){
    for(int i=0; i<64; i++){
        CipherBlocks[0][IPTable[i]-1] = (int)input[i]-48;
    }
    for(int j=0; j<32;j++){
        RRound[j]=CipherBlocks[0][j];
    }
}

int finalPermutation(){
    for(int i=0; i<64; i++){
        FPText[0][FPTable[i]-1] = CipherBlocks[0][i];
    }
}
int roundFunction(int round){
    // Expand R
    ExpansionDBox();
    // XOR R to Key for round
        // --! TODO
    // Split into 8
    SBoxSplit();
        // DONE Each 6 digits in SBox Permutation
    
    
    // Convert decimal to binary
    // Store each bit to Temp SBOx Output
    // Store each temp sbox output to RRound
    // Xor to LRound
    // Swap LRound and RRound
}
char* des(char input[], char key[]){
    initialPermutation(input);
    for(int j=0; j<32; j++){
        printf("%d",RRound[j]);
    }
    for(int i=0; i<1; i++){
        roundFunction(i);
    }
    finalPermutation();

    return "Hello";
}

int main(){
    char input[] = "1110110111001010011011000111010000100010100101111000001001100001";
    //"1011101100010011111000110111000101100011000101010111010011111101";
    char key[] = "0001001100110100010101110111100110011011101111001101111111110001";
    printf("%s\n", input);
    des(input,key);

    // for (int i = 0; i < 64; i++){
    //     printf("%d",CipherBlocks[0][i]);
    // }
    // printf("\n");
    
    // for (int i = 0; i < 64; i++){
    //     printf("%d",FPText[0][i]);
    // }
    //printf("\n");
    //printf("Cipher Text: %s\n", ciphertext);
    // printf("S-box: %d\n",Sboxes[5][3][15]);
}