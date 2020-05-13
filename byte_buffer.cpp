
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "byte_buffer.h"

BitBuffer::BitBuffer() = default;

BitBuffer::BitBuffer(BitBuffer const& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    bit_position = other.bit_position;
    buffer = (byte*) malloc(allocated_size);
    memcpy(buffer, other.buffer, size);
}

BitBuffer::BitBuffer(BitBuffer&& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    buffer = other.buffer;
    bit_position = other.bit_position;
    other.size = other.allocated_size = 0;
    other.buffer = nullptr;
}

BitBuffer& BitBuffer::operator=(BitBuffer const& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    bit_position = other.bit_position;
    buffer = (byte*) malloc(allocated_size);
    memcpy(buffer, other.buffer, size);
    return *this;
}

BitBuffer& BitBuffer::operator=(BitBuffer&& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    buffer = other.buffer;
    bit_position = other.bit_position;
    other.size = other.allocated_size = 0;
    other.buffer = nullptr;
    return *this;
}

void BitBuffer::ensure_size(size_t size) {
    if (size > allocated_size * 8) {
        while (size > allocated_size * 8) {
            allocated_size += REALLOCATION_SIZE;
        }
        if (buffer != nullptr) {
            buffer = (byte*) realloc(buffer, allocated_size);
        } else {
            buffer = (byte*) malloc(allocated_size);
        }
    }
}

byte* BitBuffer::getBuffer() const {
    return buffer;
}

size_t BitBuffer::length_bits() const {
    return size;
}

size_t BitBuffer::length_bytes() const {
    return (size + 7) / 8;
}

void BitBuffer::write_bytes(byte const *buffer, size_t size) {
    ensure_size(this->size + size);
    this->size = ((this->size + 7) / 8) * 8;
    memcpy(this->buffer + this->size / 8, buffer, size);
    this->size += size * 8;
}

void BitBuffer::write_bytes(const char* str) {
    write_bytes((const byte *) str, strlen(str));
}

void BitBuffer::write_bytes(std::string const& str) {
    write_bytes((const byte *) str.data(), str.size());
}

void BitBuffer::write_buffer(BitBuffer const& buffer) {
    if (length_bits() * 8 == length_bytes()) {
        write_bytes(buffer.buffer, buffer.size);
    }
    for (int i = 0; i < buffer.length_bytes(); i++) {
        byte b = buffer.buffer[i];
        for (int j = 0; j < 8; j++) {
            write_bit((b >> j) & 1);
        }
        write_bit(buffer.buffer[i]);
    }
}

void BitBuffer::write_bit(bit b) {
    ensure_size(size + 1);
    if (length_bits() * 8 == length_bytes()) {
        write_byte((bit) b);
        size -= 7;
    } else {
        size_t last = size / 8;
        size_t pos = size % 8;
        buffer[last] = (byte) ((buffer[last] & ~(1 << pos)) | (b << pos));
        size++;
    }
}

void BitBuffer::clear() {
    if (buffer != nullptr) {
        free(buffer);
        buffer = nullptr;
    }
    size = allocated_size = 0;
}

void BitBuffer::write_byte(byte b) {
    this->size = ((this->size + 7) / 8) * 8;
    ensure_size(size + 8);
    buffer[size / 8] = b;
    size += 8;
}

bit BitBuffer::get() const {
    int byte_pos = bit_position / 8;
    int bit_pos = bit_position % 8;
    return (bit) (((buffer[byte_pos]) >> bit_pos) & 1);
}

byte BitBuffer::get_byte() const {
    return buffer[bit_position / 8];
}

int BitBuffer::set_position(int position) {
    return bit_position = position;
}

int BitBuffer::move(int count) {
    return set_position(bit_position + count);
}

bit BitBuffer::next() {
    bit b = get();
    bit_position++;
    return b;
}

byte BitBuffer::next_byte() {
    bit_position = ((bit_position + 7) / 8) * 8;
    byte b = get_byte();
    bit_position = ((bit_position + 8) / 8) * 8;
    return  b;
}

void BitBuffer::rewind() {
    bit_position = 0;
}

bool BitBuffer::end() const {
    return bit_position >= size;
}

void BitBuffer::print() const {
    if (buffer == nullptr) {
        return;
    }

    for (int i = 0; i < size / 8; i++) {
        byte b = buffer[i];
        for (int j = 0; j < 8; j++) {
            std::cout << ((b >> j) & 1);
        }
    }
    if (size % 8 != 0) {
        byte last = buffer[size / 8];
        for (int j = 0; j < size % 8; j++) {
            std::cout << ((last >> j) & 1);
        }
    }
}

BitBuffer::~BitBuffer() {
    clear();
}