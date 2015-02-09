//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "st3bitReg.h"
#include "stMemList.h"
#include "stOpRow.h"

class stListHandle
{

private:

    // a list containing the history of all memory accesses
    stMemList memList;
    stOpRow accessTypeTable;

public:
    // saves a new memory access to the list
    int saveToMemList(st3bitReg iType, st12bitReg iAddy);
    // saves the history of memory accesses to a file
    void exportMemTrace();
    // records a memory access as a read
    void writeDataRead();
    // records a memory access as a write
    void writeDataWrite();
    // records a memory access as a fetch
    void writeDataFetch();
    
};

