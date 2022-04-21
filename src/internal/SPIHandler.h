#pragma once

#include <stdint.h>
#include <stdlib.h>

class SPIHandler
{
public:
    SPIHandler();
    ~SPIHandler();
    int begin(char* spiFilename, uint32_t speed);
    void end();
    void transfer(uint8_t *data, size_t length);

private:
    int mSPIfd;
    int mSpeed;
};
