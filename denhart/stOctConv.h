//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define OCT_MAX 8
#define OCT_0 0
#define OCT_1 1
#define OCT_2 2
#define OCT_3 3
#define OCT_4 4
#define OCT_5 5
#define OCT_6 6
#define OCT_7 7

#define OCT_3BIT 3

class stOctConv
{

private:

    unsigned int m_iOctTable[OCT_MAX];
    char m_sOctTable[OCT_MAX];
    bool m_bOctTable[OCT_MAX][OCT_3BIT];

    int getLength(bool* inReg);

public:

    stOctConv();
    ~stOctConv();
    int convToString(bool* inReg, char* rOutReg);
    char convToString(bool* inReg);
    int convToString(unsigned int inReg, char* rOutReg);
    char convToString(unsigned int inReg);
    int convToNumber(bool* inReg, unsigned int* rOutReg);
    unsigned int convToNumber(bool* inReg);
    int convToNumber(char inReg, unsigned int* rOutReg);
    unsigned int convToNumber(char inReg);
    int convToBinary(char inReg, bool* rOutReg);
    bool* convToBinary(char inReg);
    int convToBinary(unsigned int inReg, bool* rOutReg);
    bool* convToBinary(unsigned int inReg);
};