//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"


#define BYTE_MASK   0x3F //mask that saves out the lowest 6 bits of a byte
#define ADDY_MASK   0x40 //a mask that checks if the msb of a byte is an address


class stReadFile
{

private:

    // the file name
    char* sFile;
    // the upper byte read from the file
    st12bitReg rHighByte;
    // the lower byte read from a file
    st12bitReg rLowByte;

public:

    int readFile(char* sFileName);
    // applies a bit mask to save the lowest 6 bits of the byte sent in
    st12bitReg applyMask(st12bitReg rByteIn);
    // returns a 12 bit memory address from a high byte and low byte
    int calculateMemAddy(st12bitReg iHigh, st12bitReg iLow);
    // checks the upper two bits of a byte for the address code. Returns true if an address, false if data
    bool checkForAddy(st12bitReg iAddy);

};

