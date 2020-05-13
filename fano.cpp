#include "fano.h"

namespace FanoEncoder {
    BitBuffer encode(std::string text) {
        EncodingTree tree;
        tree.fromString(text);
        BitBuffer buff;
        tree.write_tree(buff);
        tree.encode_string(text, buff);
        return buff;
    }

    std::string decode(BitBuffer& buffer) {
        buffer.rewind();
        EncodingTree tree;
        tree.read_tree(buffer);
        return tree.decode_string(buffer);
    }
};