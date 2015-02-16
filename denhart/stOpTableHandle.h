//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern class stOpRow;
extern class stBitReg;

#define OP_TABLE_LENG 12


class stOpTableHandle
{

private:
    
    stOpRow m_opTable[OP_TABLE_LENG];

public:
    
    int getTableData(char* sName, int iCount);
    void fillOpTable();
    char* getMnemonic();
    int getTotal();
    void incrementTotal(stBitReg OpCode);
    int sumCycles();

};

