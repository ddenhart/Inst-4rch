//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"



class stMajorStateGen
{

private:


public:

    st12bitReg fetchState(st12bitReg rMemLocation);
    void determineInstType();
    void deferState();
    void executeState();
    void dmaState();

};

