//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "stMA.h"
#include "stMemTable.h"
#include "stMemList.h"
#include "stMB.h"
#include "stMRI.h"


class stMemoryUnit
{
    stMA rCPMA;
    stMemTable memoryTable;
    stMemList memHistory;
    stMB rMB;
    stMRI rMRI;

public:

    void receiveDCA();
    void receiveISZ(int iIndex, st12bitReg rInReg);
    void receiveJMS();
    void receiveI();
    void sense();
    void inhibit();

};

