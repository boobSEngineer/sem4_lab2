#include <iostream>
#include "fano.h"


int main() {
    std::string text = "In static member function 'static int* EncodingTree::textToCharMap(std::__cxx11::string)':";

    BitBuffer buff = FanoEncoder::encode(text);
    std::cout << FanoEncoder::decode(buff);

    return 0;
}