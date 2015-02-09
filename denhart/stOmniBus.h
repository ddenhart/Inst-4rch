//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "stInOutput.h"
#include "stAccumulator.h"
#include "stControlUnit.h"
#include "stMemoryUnit.h"



class stOmniBus
{
    
private:

    // the starting address of memory
    int rStartAddy;
    // a table of errors indexed by code
    char **errorTable;
    // the length of the error table
    int iErrorLeng;
    stInOutput IOUnit;
    stAccumulator ALUnit;
    stControlUnit ControlUnit;
    stMemoryUnit MemoryUnit;
    stAccumulator stAccumulator;
    stMicro2Cont stMicro2Cont;
    stControlUnit stControlUnit;
    stMemoryUnit stMemoryUnit;
    // populates the error table with printable errors

public:

    void fillErrorTable();
    // points to the next address in the memory table
    void incrementCurrentAddy();
    // sends address to be held at the CPMA while waiting for following data
    int getAddy(st12bitReg rAddy);
    // sends data to be held in the MB
    int getData(st12bitReg rData);
    void busRegLoads();

};

