//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern class stBitReg;
extern class stOctConv;


class stOpRow
{

private:

    int m_iOpcodeLength;
    int m_iTotalCount;
    char* m_sMnemonic;
    int m_iCycles;
    stBitReg m_rOpcode;
    
    void fillOpRow();
    void clearOpRow();
    
public:

    stOpRow();
    stOpRow(int total, char* mnem, int cycles, unsigned int index);
    ~stOpRow();
    char* getName();
    int getOpcode();
    int getCycles();
    int getTotal();
};

