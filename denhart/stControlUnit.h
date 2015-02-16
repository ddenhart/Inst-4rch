//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern class stBitReg;

class stControlUnit
{

private:

    stBitReg r12bit_PC;
    stBitReg r3bit_IR;

public:

    void opJMP(stBitReg rInReg);
    void opISZ(stBitReg rInReg);
    void loadNextInstruction();
    void opJMS(stBitReg rInReg);
    void instructionDecoder();

};

