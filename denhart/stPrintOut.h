//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "st3bitReg.h"



class stPrintOut
{

private:

    int iTotalInst;
    int iTotalCycles;

public:

    int printSummary();
    char* printMemData(st3bitReg iType, st12bitReg rAddy);
    void printError(int iError);

};

