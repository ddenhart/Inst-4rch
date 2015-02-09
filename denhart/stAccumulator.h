//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "stMicro1.h"
#include "stMicro2Acc.h"


class stAccumulator
{

private:

    st12bitReg rAC;
    bool bLink;
    stMicro1 micro1;
    stMicro2Acc micro2;
    st12bitReg rMQ;

public:

    void setLinkBit();
    void getLinkBit();
    st12bitReg sum(st12bitReg rInReg);
    void clearLinkBit();
    void clearAC();
    st12bitReg opAND(st12bitReg rInReg);
    void opTAD(st12bitReg rInReg);
    void handleCarry();
    void carryOrOverflow();
    void setAC();
    void getAC();
    void opDCA(st12bitReg rInReg);
    bool receiveISZ(st12bitReg rInReg);
    void opEAE();
};

