//------------------------------------------------------------------------------


//a char is 3 bits
//a bool is 1 bit
//a unsigned in is in decimal
//------------------------------------------------------------------------------


#define MASK0 0x00F
#define MASK1 0x0F0
#define MASK2 0xF00

class stBitReg
{
private:

    int m_iLength;
    bool* m_rBitReg;

    stBitReg copyReg(stBitReg rSource);
    stBitReg copyReg(bool* rSource);
    stBitReg copyReg(char* rSource);
    stBitReg copyReg(unsigned int rSource);
    int save3bit(bool* bReg, int index);

public:

    stBitReg();  //constructor
    stBitReg(int regSize);  //constructor for empty register
    stBitReg(stBitReg rSource, int regSize); //constructor with int
    stBitReg(bool* rSource, int regSize); //constructor with bool
    stBitReg(char* rSource, int regSize); //constructor with char
    stBitReg(unsigned int rSource, int regSize); //constructor with int
    ~stBitReg();
    void clearReg();
    int getLength();
    int getLength(stBitReg inReg);
    int getLength(bool* inReg);
    int getLength(char* inReg);
    int getLength(unsigned int inReg);
    int setReg(stBitReg rInReg);
    int setReg(bool* rInReg);
    int setReg(char* rInReg);
    int setReg(unsigned int rInReg);
    stBitReg getReg();
    bool* getBool();
    unsigned int getNumber();
    unsigned int getNumber(bool* rInReg);
    unsigned int getNumber(char* rInReg);
    char* getString();
    char* getString(stBitReg rInReg);
    char* getString(bool* rInReg);
    char* getString(unsigned int rInReg);
    stBitReg sliceBits(int iBegin, int iEnd);
    stBitReg ccatRegs(stBitReg* rSource, int iNumberWords);
};

