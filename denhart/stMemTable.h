//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "st7bitReg.h"
#include "st5bitReg.h"
#include "st3bitReg.h"
#include "stMemTableRow.h"


#define TOTAL_ROWS 4096
#define TOTAL_PAGES
#define TOTAL_WORDS
#define PAGE_MASK   0xF80   // mask to save out 5 msb of 12 bit address
#define OFFSET_MASK 0x07F   // mask to save out the last 7 lsb of 12 bits

class stMemTable
{

private:
       
    stMemTableRow memArray[TOTAL_ROWS];
    int iValidCount;
    void initializeTable();
    void setValid();

public:

    // returns 5 bit page number
    int getPage(st5bitReg iAddy);
    // returs 7 bit offset
    int getOffest(st7bitReg iAddy);
    // stores a given data at a given address
    int writeData(st3bitReg iAddy, st12bitReg iData);
    // returns data from a given address
    int readData(st12bitReg iAddy);
    void clearValid();
    void calculatePage();
    void findValid(st12bitReg *validList);

};

