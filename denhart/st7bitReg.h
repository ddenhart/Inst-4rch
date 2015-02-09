//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

#define REG_7BIT 7

class st7bitReg
{

private:

    bool r7BitReg[REG_7BIT];

public:

    void setReg();
    void getReg();
    void getPointer();
    void clearReg();
    void getHex();
    void getDecimal();
    void setHex();
    void setDecimal();
    void loadGate();
    void adderShifter();

};

