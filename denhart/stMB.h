//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"



class stMB
{

private:

    st12bitReg rMB;
    st12bitReg st12bitReg;

public:

    void writeHistory();
    void setMB();
    void getMB();
    void updatePC();
    void signalIR();
    void signalMA();

};

