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

        bool make_children(RbMap<unsigned char, BitBuffer>& character_codes) {
            delete(left); left = nullptr;
            delete(right); right = nullptr;
            if (weighted_chars.length() > 1) {
                right = new Node();
                right->code = code;
                right->code.write_bit(1);
                left = new Node();
                left->code = code;
                left->code.write_bit(0);

                float total_weight = 0;
                for (int i = 0; i < weighted_chars.length(); i++) {
                    total_weight += weighted_chars[i].second;
                }
                float weight = 0;
                for (int i = 0; i < weighted_chars.length(); i++) {
                    weight += weighted_chars[i].second;
                    (i == 0 || weight < total_weight / 2 ? right : left)->weighted_chars.add(weighted_chars[i]);
                }
                return true;
            } else {
                character_codes.insert(weighted_chars[0].first, code);
                return false;
            }
        }

        void debug_print(int intend = 0) {
            if (left != nullptr) {
                left->debug_print(intend + 1);
            }
            for (int i = 0; i < intend; i++) {
                std::cout << "    ";
            }
            if (weighted_chars.length() == 1) {
                std::cout << weighted_chars[0].first << " " << weighted_chars[0].second << "\n";
            } else if (decode_char != 0) {
                std::cout << decode_char << "\n";
            } else {
                std::cout << "#\n";
            }
            if (right != nullptr) {
                right->debug_print(intend + 1);
            }
        }

        void build_encoding_tree(RbMap<unsigned char, BitBuffer>& character_codes) {
            if (make_children(character_codes)) {
                right->build_encoding_tree(character_codes);
                left->build_encoding_tree(character_codes);
            }
        }

        void write(BitBuffer& buff) {
            if (left != nullptr) {
                buff.write_bit(1);
                left->write(buff);
            } else {
                buff.write_bit(0);
            }
            if (right != nullptr) {
                buff.write_bit(1);
                right->write(buff);
            } else {
                buff.write_bit(0);
            }
            if (weighted_chars.length() == 1) {
                buff.write_bit(1);
                buff.write_byte(weighted_chars[0].first);
            } else {
                buff.write_bit(0);
            }
        }

        void read(BitBuffer& buff, RbMap<unsigned char, BitBuffer>& character_codes) {
            delete(left); left = nullptr;
            delete(right); right = nullptr;
            if (buff.next()) {
                left = new Node();
                left->code = code;
                left->code.write_bit(0);
                left->read(buff, character_codes);
            }
            if (buff.next()) {
                right = new Node();
                right->code = code;
                right->code.write_bit(1);
                right->read(buff, character_codes);
            }
            if (buff.next()) {
                decode_char = buff.next_byte();
                character_codes.insert(decode_char, code);
            } else {
                decode_char = 0;
            }
        }

        ~Node() {
            delete(right);
            delete(left);
        }
    };

    Node root;
    RbMap<unsigned char, BitBuffer> character_codes;

public:
    EncodingTree() {
    }

    void addChar(unsigned char c, float weight) {
        for (int i = 0; i < root.weighted_chars.length(); i++) {
            if (root.weighted_chars[i].first == c) {
                root.weighted_chars[i].second += weight;
                return;
            }
        }
        root.weighted_chars.add(std::pair<unsigned char, float>(c, weight));
    }

    void addCharMap(int* map) {
        for (int c = 0; c < 256; c++) {
            if (map[c] > 0) {
                addChar((unsigned char) c, map[c]);
            }
        }
    }

    void build() {
        character_codes.clear();
        root.build_encoding_tree(character_codes);
    }

    void debug() {
        root.debug_print();
        List<unsigned char>& keys = character_codes.get_keys();
        std::cout << "character codes:\n";
        for (auto it = keys.begin(); it != keys.end(); it++) {
            unsigned char character = *it;
            std::cout << character << " ";
            BitBuffer& code = character_codes.get(character);
            code.print();
            std::cout << "\n";
        }
    }

    static int* textToCharMap(std::string text) {
        int* map = new int[256] {0};
        for (int i = 0; i < text.length(); i++) {
            map[text[i]]++;
        }
        return map;
    }

    void fromString(std::string str) {
        addCharMap(textToCharMap(str));
        build();
    }

    void read_tree(BitBuffer& buff) {
        character_codes.clear();
        root.read(buff, character_codes);
    }

    void write_tree(BitBuffer& buff) {
        root.write(buff);
    }

    void encode_string(std::string const& str, BitBuffer& buff) {
        unsigned int length = (unsigned int) str.length();
        buff.write_byte((byte) ((length >> 24) & 0xFF));
        buff.write_byte((byte) ((length >> 16) & 0xFF));
        buff.write_byte((byte) ((length >> 8) & 0xFF));
        buff.write_byte((byte) ((length >> 0) & 0xFF));

        const char* c_str = str.data();
        for (int i = 0; c_str[i]; i++) {
            BitBuffer& code = character_codes.get((unsigned char) c_str[i]);
            code.rewind();
            for (int i = 0; i < code.length_bits(); i++) {
                buff.write_bit(code.next());
            }
        }
    }

    std::string decode_string(BitBuffer& buff) {
        BitBuffer output_string;
        unsigned int length = ((unsigned int) buff.next_byte() << 24) | ((unsigned int) buff.next_byte() << 16) | ((unsigned int) buff.next_byte() << 8) | (unsigned int) buff.next_byte();
        for (int i = 0; i < length; i++) {
            Node* node = &root;
            while (node->decode_char == 0) {
                if (buff.next()) {
                    node = node->right;
                } else {
                    node = node->left;
                }
            }
            output_string.write_byte(node->decode_char);
        }
        output_string.write_byte(0);
        return std::string((char*) output_string.getBuffer());
    }
};


#endif //SEM4LAB2_TREE_H
