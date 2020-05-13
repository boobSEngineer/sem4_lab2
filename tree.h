#include "rb_map.h"
#include "array.h"
#include "byte_buffer.h"

#ifndef SEM4LAB2_TREE_H
#define SEM4LAB2_TREE_H

class EncodingTree {
public:
    class Node {
    public:
        Array<std::pair<unsigned char, float>> weighted_chars;
        BitBuffer code;
        unsigned char decode_char = 0;

        Node* right = nullptr;
        Node* left = nullptr;

        bool make_children(RbMap<unsigned char, BitBuffer>& character_codes);
        void debug_print(int intend = 0);
        void build_encoding_tree(RbMap<unsigned char, BitBuffer>& character_codes);
        void write(BitBuffer& buff);
        void read(BitBuffer& buff, RbMap<unsigned char, BitBuffer>& character_codes);

        ~Node();
    };

    Node root;
    RbMap<unsigned char, BitBuffer> character_codes;

public:
    EncodingTree();
    void addChar(unsigned char c, float weight);
    void addCharMap(int* map);
    void build();
    void fromString(std::string str);

    void debug();
    int calculate_encoded_size();

    void read_tree(BitBuffer& buff);
    void write_tree(BitBuffer& buff);
    void encode_string(std::string const& str, BitBuffer& buff);
    std::string decode_string(BitBuffer& buff);

    static int* textToCharMap(std::string text);
};


#endif //SEM4LAB2_TREE_H
