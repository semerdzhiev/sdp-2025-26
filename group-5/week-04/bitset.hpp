#pragma once

#include <vector>
#include <iostream>
#include <cstdint>

class BitSet{
private:
    std::vector<uint8_t> values;
    size_t bit_size;

    size_t byte_index(size_t bit_index) const {
        return bit_index / 8;
    }

public:
    BitSet() : bit_size(0) {}

    explicit BitSet(size_t size) : bit_size(0) 
    {
        values.reserve((size / 8) + 1);
    }

    class BitProxy 
    {
        private:
            uint8_t& byteRef;
            size_t bitPos; // 0 <= bitPos < 8
            public:
            BitProxy(uint8_t& byteRef, size_t bitPos) : byteRef(byteRef), bitPos(bitPos) {};
            BitProxy(const BitProxy& other) = default;

            operator bool()
            {
                uint8_t mask = 1 << bitPos;
                return (byteRef & mask) != 0;  
            }
            
            BitProxy& operator=(bool value)
            {
                uint8_t mask = 1 << bitPos;

                if (value) {
                    byteRef |= mask;  
                } else {
                    byteRef &= ~mask; 
                }
                
                // std::cout << "Pushed bit: " << value 
                //         << "At bitPos " << bitPos << std::endl;

                return *this;
            }
    };

    BitProxy operator[](size_t index)
    {
        if (index >= bit_size) {
            throw std::out_of_range("BitSet index out of range");
        }

        this->getElement(index);
    }

    void push_back(bool value) 
    {
        if (bit_size % 8 == 0) {
            values.push_back(0);
        }

        this->getElement(bit_size) = value;
        bit_size++;
    }

    void pop_back();
    size_t size() const {
        return bit_size;
    }
    void flip();
    void flip(size_t pos);
    bool all()const;
    bool any()const;
    bool none()const;

private:
    BitProxy getElement(size_t index)
    {   
        // std::cout << "Getting element at index: " << index << std::endl;
        size_t byteIdx = byte_index(index);
        size_t bitPos = index % 8;
        return BitProxy(values[byteIdx], bitPos);
    }
};
