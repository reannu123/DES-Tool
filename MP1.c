#include <stdio.h>

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
    // Initial Permutation
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
    // Final Permutation



int main(){
    char input[21] = "";
}