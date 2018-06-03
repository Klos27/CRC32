#ifndef CRC_H
#define CRC_H
#include <iostream>

class CRC {
private:

public:
    std::string countCRC(std::string filePath);
    std::string countCRCviaTab(std::string filePath);
};


#endif // CRC_H
