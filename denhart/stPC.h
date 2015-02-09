//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"


class stPC
{

private:

    st12bitReg rPC;

public:

    void nextInstruction();
    void loadBranchAddy(st12bitReg rInReg);

};

