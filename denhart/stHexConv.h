//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define HEX_MAX 16
#define HEX_0 0x0
#define HEX_1 0x1
#define HEX_2 0x2
#define HEX_3 0x3
#define HEX_4 0x4
#define HEX_5 0x5
#define HEX_6 0x6
#define HEX_7 0x7
#define HEX_8 0x8
#define HEX_9 0x9
#define HEX_A 0xA
#define HEX_B 0xB
#define HEX_C 0xC
#define HEX_D 0xD
#define HEX_E 0xE
#define HEX_F 0xF


class stHexConv
{

private:

    unsigned int m_iHexTable[HEX_MAX];
    char m_sHexTable[HEX_MAX];
    bool m_bHexTable[HEX_MAX][HEX_4BIT];

    int getLength(bool* inReg);

public:

    stHexConv();
    ~stHexConv();
    int convToString(bool* inReg, char* rOutReg);
    int convToString(unsigned int inReg, char* rOutReg);
    int convToNumber(bool* inReg, unsigned int* rOutReg);
    int convToNumber(char inReg, unsigned int* rOutReg);
    int convToBinary(char inReg, bool* rOutReg);
    int convToBinary(unsigned int inReg, bool* rOutReg);

};