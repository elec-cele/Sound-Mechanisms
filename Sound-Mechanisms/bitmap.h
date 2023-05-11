#include <iostream>
#include "sound.h"
#include <fstream>
#include <vector>
const extern double two_pi;
const extern int SAMPLE_RATE;

typedef unsigned char byte;


//Doesn't work at all -- 
// idea is for drawing wavetables as 1 bit per pixel bitmaps

std::string read_binary(const char* filename) {
    
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    std::string mystring;
    if (file.is_open()) { // always check whether the file is open
        file >> mystring; // pipe file's content into stream
        std::cout << mystring; // pipe stream's content to standard output
    }
    return mystring;
}