//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
extern class stHexConv;

#define REG_12BIT 12
#define MASK0 0x00F;
#define MASK1 0x0F0;
#define MASK2 0xF00;

class st12bitReg
{

private:

    bool m_r12BitReg[REG_12BIT];
    stHexConv m_HexTable;

    int copyReg(st12bitReg rSource, st12bitReg* rDest);
    int copyReg(bool* rSource, st12bitReg* rDest);
    int getLength(bool* inReg);
    int get4Bits(bool* rSource, bool* rDest, int iBegin, int iEnd);
    int setBinary(bool* rIn1, bool* rIn2, bool* rIn3, bool* rOut);

public:

    st12bitReg();
    st12bitReg(bool* rSource);
    ~st12bitReg();
    int setReg(st12bitReg rInReg);
    int setReg(bool* rInReg);
    int getReg(st12bitReg* rOutReg);
    void clearReg();
    int getHex(unsigned int* rOutReg);
    int getString(char* rOutReg);
    int setHex(unsigned int rOutReg);
    int setString(char* rOutReg);
    //int loadGate();
    //int adderShifter();

};

