//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st3bitReg.h"


class stOpRow
{

private:

    int iTotalCount;
    char* sMnemonic;
    int iCycles;
    st3bitReg rOpcode;
    bool bIsIndirect;
    bool bAutoIncrement;

public:

    char* sendName(int iIndex);
    int sendType(char* sName);
    void fillTypeTable();

};

