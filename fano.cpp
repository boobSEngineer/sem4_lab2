#include "fano.h"

namespace FanoEncoder {
    BitBuffer encode(std::string text, bool verbose, bool verbose_tree) {
        if (verbose) {
            std::cout << "--------------- DEBUG START --------------\n";
            std::cout << "encoder input: " << text << "\n";
            std::cout << "input size (bits): " << text.length() * 8 << "\n";
        }

        BitBuffer buff;
        if (text.empty()) {
            buff.write_bit(0);
            if (verbose) {
                std::cout << "empty string input, returning buffer with single zero bit\n";
                std::cout << "\n------------------- END ------------------\n\n";
            }
            return buff;
        } else {
            buff.write_bit(1);
        }

        EncodingTree tree;
        tree.fromString(text);
        int theory_size = tree.calculate_encoded_size();
        if (verbose) {
            std::cout << "encoded size (no dictionary): " << theory_size << "\n";
        }
        if (verbose_tree) {
            tree.debug();
        }
        tree.write_tree(buff);
        if (verbose) {
            std::cout << "dictionary size: " << buff.length_bits() - 1 << "\n";
        }
        tree.encode_string(text, buff);
        if (verbose) {
            std::cout << "encoded size (real, with saved string size and aligned bytes): " << buff.length_bits() << "\n";
            std::cout << "encoded size / real size: " << 100.0 * buff.length_bits() / (text.length() * 8.0) << "%\n";
            std::cout << "encoded buffer bits: ";
            buff.print();
            std::cout << "\n------------------- END ------------------\n\n";
        }

        return buff;
    }

    std::string decode(BitBuffer& buffer) {
        buffer.rewind();
        if (!buffer.next()) {
            return "";
        }
        EncodingTree tree;
        tree.read_tree(buffer);
        return tree.decode_string(buffer);
    }
};