#include <iostream>
#include "bitset.hpp"

int main() {
    BitSet bits(10);
    bits.push_back(true);
    bits.push_back(false);
    bits.push_back(true);
    bits.push_back(true);
    bits.push_back(false);
    bits.push_back(false);
    bits.push_back(true);
    bits.push_back(false);
    bits.push_back(true);
    bits.push_back(true);

    std::cout << "BitSet's 4th element: " << bits[3] << std::endl;
    std::cout << "BitSet's 5th element: " << bits[4] << std::endl;

    return 0;
}