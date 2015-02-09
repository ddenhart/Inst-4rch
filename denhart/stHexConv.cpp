#include "stHexConv.h"
#include "Common.h"
#include <cstdio>


//constructor
stHexConv::stHexConv()
{
    m_iHexTable[0] = HEX_0;
    m_iHexTable[1] = HEX_1;
    m_iHexTable[2] = HEX_2;
    m_iHexTable[3] = HEX_3;
    m_iHexTable[4] = HEX_4;
    m_iHexTable[5] = HEX_5;
    m_iHexTable[6] = HEX_6;
    m_iHexTable[7] = HEX_7;
    m_iHexTable[8] = HEX_8;
    m_iHexTable[9] = HEX_9;
    m_iHexTable[10] = HEX_A;
    m_iHexTable[11] = HEX_B;
    m_iHexTable[12] = HEX_C;
    m_iHexTable[13] = HEX_D;
    m_iHexTable[14] = HEX_E;
    m_iHexTable[15] = HEX_F;

    m_sHexTable[0] = '0';
    m_sHexTable[1] = '1';
    m_sHexTable[2] = '2';
    m_sHexTable[3] = '3';
    m_sHexTable[4] = '4';
    m_sHexTable[5] = '5';
    m_sHexTable[6] = '6';
    m_sHexTable[7] = '7';
    m_sHexTable[8] = '8';
    m_sHexTable[9] = '9';
    m_sHexTable[10] = 'A';
    m_sHexTable[11] = 'B';
    m_sHexTable[12] = 'C';
    m_sHexTable[13] = 'D';
    m_sHexTable[14] = 'E';
    m_sHexTable[15] = 'F';

    m_bHexTable[0][0] = 0;
    m_bHexTable[0][1] = 0;
    m_bHexTable[0][2] = 0;
    m_bHexTable[0][3] = 0;

    m_bHexTable[1][0] = 1;
    m_bHexTable[1][1] = 0;
    m_bHexTable[1][2] = 0;
    m_bHexTable[1][3] = 0;

    m_bHexTable[2][0] = 0;
    m_bHexTable[2][1] = 1;
    m_bHexTable[2][2] = 0;
    m_bHexTable[2][3] = 0;

    m_bHexTable[3][0] = 1;
    m_bHexTable[3][1] = 1;
    m_bHexTable[3][2] = 0;
    m_bHexTable[3][3] = 0;

    m_bHexTable[4][0] = 0;
    m_bHexTable[4][1] = 0;
    m_bHexTable[4][2] = 1;
    m_bHexTable[4][3] = 0;

    m_bHexTable[5][0] = 1;
    m_bHexTable[5][1] = 0;
    m_bHexTable[5][2] = 1;
    m_bHexTable[5][3] = 0;

    m_bHexTable[6][0] = 0;
    m_bHexTable[6][1] = 1;
    m_bHexTable[6][2] = 1;
    m_bHexTable[6][3] = 0;

    m_bHexTable[7][0] = 1;
    m_bHexTable[7][1] = 1;
    m_bHexTable[7][2] = 1;
    m_bHexTable[7][3] = 0;

    m_bHexTable[8][0] = 0;
    m_bHexTable[8][1] = 0;
    m_bHexTable[8][2] = 0;
    m_bHexTable[8][3] = 1;

    m_bHexTable[9][0] = 1;
    m_bHexTable[9][1] = 0;
    m_bHexTable[9][2] = 0;
    m_bHexTable[9][3] = 1;

    m_bHexTable[10][0] = 0;
    m_bHexTable[10][0] = 1;
    m_bHexTable[10][0] = 0;
    m_bHexTable[10][0] = 1;

    m_bHexTable[11][0] = 1;
    m_bHexTable[11][0] = 1;
    m_bHexTable[11][0] = 0;
    m_bHexTable[11][0] = 1;

    m_bHexTable[12][0] = 0;
    m_bHexTable[12][0] = 0;
    m_bHexTable[12][0] = 1;
    m_bHexTable[12][0] = 1;

    m_bHexTable[13][0] = 1;
    m_bHexTable[13][0] = 0;
    m_bHexTable[13][0] = 1;
    m_bHexTable[13][0] = 1;

    m_bHexTable[14][0] = 0;
    m_bHexTable[14][0] = 1;
    m_bHexTable[14][0] = 1;
    m_bHexTable[14][0] = 1;

    m_bHexTable[15][0] = 1;
    m_bHexTable[15][0] = 1;
    m_bHexTable[15][0] = 1;
    m_bHexTable[15][0] = 1;

}


//deconstructor
stHexConv::~stHexConv()
{

}


//returns the length of the boolean register
int stHexConv::getLength(bool* rInReg)
{
    int count = 0;

    while(rInReg)
    {
        ++count;
        ++rInReg;
    }

    return count;
}


 int stHexConv::convToString(bool* rInReg, char* rOutReg)
 {
    int error = ERROR_NONE;
    int iLength = getLength(rInReg);
    int i = 0;
    bool bFound = false;

    if(iLength == HEX_4BIT)
    {
        for(i = 0; i < HEX_MAX; ++i)
        {
            if(rInReg[0] == m_bHexTable[i][0])
            {
                if(rInReg[1] == m_bHexTable[i][1])
                {
                    if(rInReg[2] == m_bHexTable[i][2])
                    {
                        if(rInReg[3] == m_bHexTable[i][3])
                        {
                            bFound = true;
                        }
                    }
                }
            } //else not this row, keep looking
        }

        if(bFound)
        {
            if(rOutReg)
            {
                *rOutReg = m_sHexTable[i];
            }
            else
            {
                DEBUG_ERROR("HexConv: output structure is NULL");
                error = ERROR_HEXCONV_NULL;
            }
        }
        else
        {
            DEBUG_ERROR("HexConv: Couldn't find integer value");
            error = ERROR_HEXCONV_OPP_FAILED;
        }
    }
    else
    {
        DEBUG_ERROR("HexConv: Wrong input hex length");
        error = ERROR_HEXCONV_OPP_FAILED;
    }

    return error;
}


 int stHexConv::convToString(unsigned int rInReg, char* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

    for(i = 0; i < HEX_MAX; ++i)
    {
        if(rInReg == m_iHexTable[i])
        {
            bFound = true;
        } //else not this row, keep looking
    }

    if(bFound)
    {
        if(rOutReg)
        {
            *rOutReg = m_sHexTable[i];
        }
        else
        {
            DEBUG_ERROR("HexConv: output structure is NULL");
            error = ERROR_HEXCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("HexConv: Couldn't find integer value");
        error = ERROR_HEXCONV_OPP_FAILED;
    }

    return error;
}


 int stHexConv::convToNumber(bool* rInReg, unsigned int* rOutReg)
 {
    int error = ERROR_NONE;
    int iLength = getLength(rInReg);
    int i = 0;
    bool bFound = false;

    if(iLength == HEX_4BIT)
    {
        for(i = 0; i < HEX_MAX; ++i)
        {
            if(rInReg[0] == m_bHexTable[i][0])
            {
                if(rInReg[1] == m_bHexTable[i][1])
                {
                    if(rInReg[2] == m_bHexTable[i][2])
                    {
                        if(rInReg[3] == m_bHexTable[i][3])
                        {
                            bFound = true;
                        }
                    }
                }
            } //else not this row, keep looking
        }

        if(bFound)
        {
            if(rOutReg)
            {
                *rOutReg = m_iHexTable[i];
            }
            else
            {
                DEBUG_ERROR("HexConv: output structure is NULL");
                error = ERROR_HEXCONV_NULL;
            }
        }
        else
        {
            DEBUG_ERROR("HexConv: Couldn't find boolean value");
            error = ERROR_HEXCONV_OPP_FAILED;
        }
    }
    else
    {
        DEBUG_ERROR("HexConv: Wrong input hex length");
        error = ERROR_HEXCONV_OPP_FAILED;
    }

    return error;
}


 int stHexConv::convToNumber(char rInReg, unsigned int* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

    for(i = 0; i < HEX_MAX; ++i)
    {
        if(rInReg == m_sHexTable[i])
        {
            bFound = true;
        } //else not this row, keep looking
    }

    if(bFound)
    {
        if(rOutReg)
        {
            *rOutReg = m_iHexTable[i];
        }
        else
        {
            DEBUG_ERROR("HexConv: output structure is NULL");
            error = ERROR_HEXCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("HexConv: Couldn't find character value");
        error = ERROR_HEXCONV_OPP_FAILED;
    }

    return error;
}


 int stHexConv::convToBinary(char rInReg, bool* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

    for(i = 0; i < HEX_MAX; ++i)
    {
        if(rInReg == m_sHexTable[i])
        {
            bFound = true;
        } //else not this row, keep looking
    }

    if(bFound)
    {
        if(rOutReg)
        {
            for(int j = 0; j < HEX_4BIT; ++j)
            {
                if(rOutReg[j])
                {
                    rOutReg[j] = m_bHexTable[i][j];
                }
                else
                {
                    DEBUG_ERROR("HexConv: boolean array out of bounds");
                    error = ERROR_HEXCONV_OPP_FAILED;
                    break;
                }
            }
        }
        else
        {
            DEBUG_ERROR("HexConv: output structure is NULL");
            error = ERROR_HEXCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("HexConv: Couldn't find character value");
        error = ERROR_HEXCONV_OPP_FAILED;
    }

    return error;

}


 int stHexConv::convToBinary(unsigned int rInReg, bool* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

    for(i = 0; i < HEX_MAX; ++i)
    {
        if(rInReg == m_iHexTable[i])
        {
            bFound = true;
        } //else not this row, keep looking
    }

    if(bFound)
    {
        if(rOutReg)
        {
            for(int j = 0; j < HEX_4BIT; ++j)
            {
                if(rOutReg[j])
                {
                    rOutReg[j] = m_bHexTable[i][j];
                }
                else
                {
                    DEBUG_ERROR("HexConv: boolean array out of bounds");
                    error = ERROR_HEXCONV_OPP_FAILED;
                    break;
                }
            }
        }
        else
        {
            DEBUG_ERROR("HexConv: output structure is NULL");
            error = ERROR_HEXCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("HexConv: Couldn't find integer value");
        error = ERROR_HEXCONV_OPP_FAILED;
    }

    return error;
}

