#include "CRC.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <cmath>
#include <iomanip>

#define POLYNOMIAL 0x04C11DB7l  // CRC-32 polynomial

uint32_t reverse32Bits(uint32_t value) {
    uint32_t result = 0l;
    for (long i = 31; i >= 0; i--) {
        result = result | ((value & 1) << i );
        value = value >> 1;
    }
    return result;
}

uint8_t reverse8Bits(uint8_t value) {
    uint8_t result = 0l;
    for (long i = 7; i >= 0; i--) {
        result = result | ((value & 1) << i );
        value = value >> 1;
    }
    return result;
}

uint32_t createAscii(uint32_t index) {
    uint32_t result = index;
    result = reverse32Bits(result);

    uint32_t firstBit = 0x80000000;  // x^31

    for (int i = 0; i < 8; i++) {
        if ((result & firstBit) != 0)   // if first bit == 1 then XOR
            result = ((result << 1) ^ POLYNOMIAL);
        else    // first bit == 0 then skip XOR
            result = result << 1;
    }

    result = reverse32Bits(result);
    return result;
}

uint32_t* createAsciiTab() {
    uint32_t* tab = new uint32_t[256];
    for (int i = 0; i < 256; i++)
        tab[i] = createAscii(i);
    return tab;
}

std::string CRC::countCRCviaTab(std::string filePath) {
    uint32_t* tab = createAsciiTab();   // create ASCII tab
    uint32_t crc = 0xFFFFFFFFl; // init CRC-32

    // open file
    std::ifstream fin;
    fin.open(filePath.c_str(), std::ios_base::in | std::ios_base::binary);

    if (fin.is_open()) {
        char ch;

        // while -> EOF
        while (fin.get(ch)) {
            crc = (tab[(int)((crc ^ ch) & 0xFF)] ^ (crc >> 8));
        }
        fin.close();
    }

    // FINAL XOR
    crc = (crc ^ 0xFFFFFFFFl);

    std::stringstream crcSum;
    crcSum << std::hex << crc;

    delete[] tab;
    return crcSum.str();
}

std::string CRC::countCRC(std::string filePath) {
    uint32_t crc = 0xFFFFFFFFl; // init CRC-32

    // open file
    std::ifstream fin;
    fin.open(filePath.c_str(), std::ios_base::in | std::ios_base::binary);

    if (fin.is_open()) {
        char ch;
        uint32_t firstBit = 0x80000000;  // x^31

        // while -> EOF
        while (fin.get(ch)) {
            uint8_t tmp = ch;
            crc ^= (reverse8Bits(tmp) << 24);

            for (int bit = 0; bit < 8; bit++) {
                if ((crc & firstBit) != 0) // if first bit == 1 then XOR
                    crc = ((crc << 1) ^ POLYNOMIAL);
                else // first bit == 0 then skip XOR
                    crc = (crc << 1);
            }
        }
        fin.close();
    }

    // FINAL XOR
    crc = (reverse32Bits(crc) ^ 0xFFFFFFFFl);
    std::stringstream crcSum;
    crcSum << std::hex << crc;

    return crcSum.str();
}
