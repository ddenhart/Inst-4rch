//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

#define REG_5BIT 5

class st5bitReg
{

private:

    bool r5BitReg[REG_5BIT];

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

