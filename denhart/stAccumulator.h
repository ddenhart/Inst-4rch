//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern class stBitReg;

#define HEX_4BIT 4


class stAccumulator
{

private:

    stBitReg r12bit_AC;
    bool bLink;
    stBitReg r12bit_MQ;

public:

    void setLinkBit();
    void getLinkBit();
    stBitReg sum(stBitReg rInReg);
    void clearLinkBit();
    void clearAC();
    stBitReg opAND(stBitReg rInReg);
    void opTAD(stBitReg rInReg);
    void handleCarry();
    void carryOrOverflow();
    void setAC();
    void getAC();
    void opDCA(stBitReg rInReg);
    bool receiveISZ(stBitReg rInReg);
    void opEAE();
};

