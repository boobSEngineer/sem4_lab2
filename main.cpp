#include <iostream>
#include <fstream>
#include "fano.h"


int main() {
    std::string input;
    std::cout << "input string or filename to encode: ";
    std::getline(std::cin, input);

    std::ifstream inFile;
    inFile.open(input);
    if (inFile) {
        input = std::string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    }

    BitBuffer buff = FanoEncoder::encode(input, true, true);
    std::cout << "\n\ndecoder output: " << FanoEncoder::decode(buff);

    return 0;
}