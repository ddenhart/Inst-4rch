//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern class st12bitReg;
extern class stHexConv;

#define REG_3BIT 3

class st3bitReg
{

private:

    bool r3BitReg[REG_3BIT];
    stHexConv m_HexTable;

    int copyReg(st12bitReg rSource, st12bitReg* rDest);
    int copyReg(bool* rSource, st12bitReg* rDest);
    int getLength(bool* inReg);

public:

    st3bitReg();
    ~st3bitReg();
    int setReg(st12bitReg rInReg);
    int setReg(bool* rInReg);
    int getReg(st12bitReg* rOutReg);
    void clearReg();
    int getHex(unsigned int* rOutReg);
    int getString(char* rOutReg);
    int setHex(unsigned int rOutReg);
    int setString(char* rOutReg);

};

