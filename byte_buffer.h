
#include <string>

#ifndef SEM4LAB2_BYTE_BUFFER_H
#define SEM4LAB2_BYTE_BUFFER_H

typedef unsigned char byte;
typedef bool bit;

class BitBuffer {
    static const size_t REALLOCATION_SIZE = 1024;

    byte* buffer = nullptr;
    size_t size = 0;
    size_t allocated_size = 0;

    int bit_position = 0;

    void ensure_size(size_t size);
public:

    BitBuffer();
    BitBuffer(BitBuffer const& other);
    BitBuffer(BitBuffer&& other);

    BitBuffer& operator=(BitBuffer const&);
    BitBuffer& operator=(BitBuffer&&);

    size_t length_bytes() const;
    size_t length_bits() const;
    byte* getBuffer() const;
    void write_bytes(byte const *buffer, size_t size);
    void write_bytes(const char *str);
    void write_bytes(std::string const &str);

    void write_byte(byte b);
    void write_bit(bit b);
    void write_buffer(BitBuffer const& buffer);

    bit get() const;
    byte get_byte() const;
    int move(int count);
    int set_position(int position);
    void rewind();
    bool end() const;

    bit next();
    byte next_byte();

    void clear();
    void print() const;

    ~BitBuffer();
};

#endif //SEM4LAB2_BYTE_BUFFER_H
