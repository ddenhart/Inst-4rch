//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "stIR.h"
#include "stMajorStateGen.h"
#include "stMicro2Cont.h"
#include "stTimer.h"

class stControlUnit
{

private:

    st12bitReg rPC;
    stIR rIR;
    stMajorStateGen stateOps;
    stMicro2Cont micro2;
    stTimer clk;

public:

    void opJMP(st12bitReg rInReg);
    void opISZ(st12bitReg rInReg);
    void loadNextInstruction();
    void opJMS(st12bitReg rInReg);
    void instructionDecoder();

};

