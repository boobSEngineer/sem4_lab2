#include <iostream>
#include "fano.h"


int main() {
    std::string text = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcd";//"In static member function 'static int* EncodingTree::textToCharMap(std::__cxx11::string)':";

    BitBuffer buff = FanoEncoder::encode(text, true, true
    );
    std::cout << FanoEncoder::decode(buff);

    return 0;
}