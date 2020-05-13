#include "array.h"
#include "tree.h"
#include "byte_buffer.h"

#ifndef SEM4LAB2_FANO_H
#define SEM4LAB2_FANO_H


namespace FanoEncoder {
    BitBuffer encode(std::string text);
    std::string decode(BitBuffer& buffer);
};

#endif //SEM4LAB2_FANO_H
