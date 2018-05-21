#include "CRC.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <cmath>
//#include <string>

using namespace std;

// change 8 * char to they binary tab representation
bool * charTabToBits(const unsigned char *tab) {
    // 8 chars for 8 bits each
    bool * result(new bool[8 * 8 * sizeof(unsigned char)]);
    for (int digitNo = 0; digitNo < 8; digitNo++) {
        unsigned char digit = tab[digitNo];
        int offset = digitNo * 8;
        for (int bitPosition = 7; bitPosition >= 0; bitPosition--) {
            digit % 2 == 0 ? result[bitPosition + offset] = false : result[bitPosition + offset] = true;
            digit /= 2;
        }
    }
    return result;
}

bool checkTabIfEmpty(bool * binaryTab) {
    for (int i = 0; i < 32; i++)
        if (binaryTab[i])
            return false;
    return true;
}

int countLength(const bool binaryTab[], const int length) {
    int offset = 0;
    for (int i = 0; i < length; i++) {
        if (binaryTab[i] == true)
            break;
        else
            offset++;
    }
    return (length - offset);
}

void convertBinaryTabToCharTab(bool * binaryTab,unsigned char * buffer) {
    for (int charNo = 0; charNo < 8; charNo++) {
        unsigned char digit = 0;
        int offset = charNo * 8;
        for (int bitPosition = 0; bitPosition < 8; bitPosition++) {
            if(binaryTab[bitPosition + offset])
                digit += pow(2, 7 - bitPosition);
        }
        buffer[charNo] = digit;
    }
}

bool xorBuffer(unsigned char *buffer) {
    bool * binaryTab = nullptr;
    try {
        // CRC-32
        // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  1  0  1  1  0  1  1  0  1  1  1
        // = 2227247543
        //const int divider = 2227247543;
        const int dividerSizeBits = 33;
        const bool binaryDivider[dividerSizeBits] = { 1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,1,1,0,1,1,1 };
        //unique_ptr<bool[]> binaryDivider = intToBits(divider);
        const int dividerLength = countLength(binaryDivider, dividerSizeBits);
        const int dividerOffset = dividerSizeBits - dividerLength;
        cout << "Divider offset (should be 5): = " << std::dec << dividerOffset << endl << "Divider length (should be 27): = " << dividerLength << endl;

        bool * binaryTab = charTabToBits(buffer);
        cout << "binaryTab : ";
        for (int k = 0; k < 64; k++) {
            cout << binaryTab[k] << ";";
        }
        cout << endl;
        // check tab if first part = 0
        while (!checkTabIfEmpty(binaryTab)) {
            // set the beginnning
            int start = 0;
            for (start = 0; start < 32; start++)
                if (binaryTab[start] == true)
                    break;
            cout << "start: " << std::dec << start << endl;
            for (int k = 0; k < 64; k++) {
                cout << binaryTab[k] << ";";
            }
            cout << endl;
            for (int k = 0; k < start; k++) {
                cout << "  ";
            }
            for (int k = 0; k < dividerLength; k++) {
                cout << binaryDivider[k + dividerOffset] << ";";
            }
            cout << endl;
            for (int i = 0; i < dividerLength; i++)
                binaryTab[i + start] = binaryTab[i + start] ^ binaryDivider[i + dividerOffset];    
        }
        cout << "binaryTab after xor: " << endl;
        for (int k = 0; k < 64; k++) {
            cout << binaryTab[k] << ";";
        }
        cout << endl;
        // binaryTab -> charTab
        convertBinaryTabToCharTab(binaryTab, buffer);
    }
    catch (exception e) {
        delete[] binaryTab;
        return false;
    }

    delete[] binaryTab;

    return true;
}

std::string CRC::countCRC(std::string fileName)
{
    // create variables
    int bufferOnePartSize = 4;
    int bufferSize = bufferOnePartSize * 3;
    bool EOFFlag = false;
    // create tab
    //unique_ptr<char[]> buffer(new char[bufferSize]);
    unsigned char *buffer = new unsigned char[bufferSize];

    //for (int i = 0; i < 2 * bufferOnePartSize; i++)
    //	buffer[i] = 0;
    //for (int i = 2 * bufferOnePartSize; i < bufferSize; i++)
    //	buffer[i] = 255;

    for (int i = 0; i < 3 * bufferOnePartSize; i++)
        buffer[i] = 0;


    // open file
    ifstream fin;
    fin.open(fileName.c_str(), ios_base::in | ios_base::binary);

    int position = 0;
    int count = 0;
    // while -> EOF
    if (fin.is_open()) {
        // TODO check if file <= 4 -> another solution

        char ch;
        int multiply = 2;
        bool skipNormalXor = false;
        while (true) {
            count = 0;
            if (multiply == 2)
                position = 0;
            else
                position = bufferOnePartSize;
            // copy part form file to tab
            cout << "Load: ";
            while (count < bufferOnePartSize * multiply && fin.get(ch)) {
                cout << std::hex << (unsigned int)ch << ":";
                buffer[position] = (unsigned char)ch;
                ++count;
                // devide part
                ++position;
            }
            cout << endl;
            // if EOF write zeros to end of tab
            if (count == 0) {
                cout << "TODO: end of file" << endl;
                EOFFlag = true;
                skipNormalXor = true;
            }
            else if (count != bufferOnePartSize * multiply) {
                for (int i = count; i < bufferOnePartSize * multiply; i++) {
                    buffer[position] = 0;
                    position++;
                }
                EOFFlag = true;
            }

            if (!skipNormalXor) {
                // -- first part
                multiply = 1;
                // xor the buffer
                cout << "\n BUFF BEFORE XOR: ";
                for (int i = 0; i < bufferSize; i++) {
                    cout << std::hex << (unsigned int)buffer[i] << ":";
                    if ((i + 1) % 4 == 0)
                        cout << " ";
                }
                cout << endl;

                xorBuffer(buffer);

                cout << "\n BUFF AFTER XOR: ";
                for (int i = 0; i < bufferSize; i++) {
                    cout << std::hex << (unsigned int)buffer[i] << ":";
                    if ((i + 1) % 4 == 0)
                        cout << " ";
                }
                cout << endl;

                cout << "END XOR \n";
            }

            if (EOFFlag) {	// xor to the end
                cout << "\n BUFF BEFORE XOR TO THE END: ";
                for (int i = 0; i < bufferSize; i++) {
                    cout << std::hex << (unsigned int)buffer[i] << ":";
                    if ((i + 1) % 4 == 0)
                        cout << " ";
                }
                cout << endl;

                xorBuffer(buffer + bufferOnePartSize);

                cout << "\n BUFF AFTER XOR TO THE END: ";
                for (int i = 0; i < bufferSize; i++) {
                    cout << std::hex << (unsigned int)buffer[i] << ":";
                    if ((i + 1) % 4 == 0)
                        cout << " ";
                }
                cout << endl;
                break;
            }
            else {
                for (int i = 0; i < bufferOnePartSize; i++)
                    buffer[i] = buffer[i + bufferOnePartSize];
            }
        }

        cout << "\n End\n";
        fin.close();
    }

    stringstream crcSum;
    for (int i = 2 * bufferOnePartSize; i < bufferSize; i++) {
        crcSum << std::hex << (unsigned int)buffer[i];
       //printf("buffer[%d] = %02x \n", i, buffer[i]);
    }
    cout << "\n CRC: " << crcSum.str() << endl;

    delete[] buffer;
    return crcSum.str();
}
