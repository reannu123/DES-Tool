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
int ExpBlock[48];
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

int Sbox1[4][16] = {
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
};

int SBoxPermutation(int SBoxInput[], int SBoxNum){
    int row = SBoxInput[0]*2 + SBoxInput[5];
    int column = SBoxInput[1]*8 + SBoxInput[2]*4 + SBoxInput[3]*2 + SBoxInput[4];


    
}

int ExpansionDBox(){
    for(int i = 0; i < 48; i++){
        ExpBlock[i] = RRound[ExpDBoxTable[i]-1];
    }
}

int initialPermutation(char input[]){
    for(int i=0; i<64; i++){
        CipherBlocks[0][IPTable[i]-1] = (int)input[i]-48;
    }
}

int finalPermutation(){
    for(int i=0; i<64; i++){
        FPText[0][FPTable[i]-1] = CipherBlocks[0][i];
    }
}

char* des(char input[], char key[]){
    initialPermutation(input);
    for(int i=0; i<16; i++){
        // round( ,)
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
    printf("S-box: %d",Sbox1[0][15]);
}