#include "st12bitReg.h"
#include "Common.h"
#include "stHexConv.h"
#include <cstdio>
#include <cstring>


//constructor
st12bitReg::st12bitReg()
{
    clearReg();
}


//constructor with initialization
st12bitReg::st12bitReg(bool* rSource)
{
    int error = ERROR_NONE;

    error = setReg(rSource);

    if(error != ERROR_NONE)
    {
        DEBUG_ERROR("12bitReg: constructor initialization failed");
    }
}


//deconstructor
st12bitReg::~st12bitReg()
{

}


//copies the register value from rSource to rDest
//st12bitReg rSource holds the register value that will be coppied
//st12bitReg* rDest holds the register value that will be set
//returns standard error
 int st12bitReg::copyReg(st12bitReg rSource, st12bitReg* rDest)
{
    int error = ERROR_NONE;

    if(rDest)
    {
        for(int i = 0; i < REG_12BIT; ++i)
        {
            rDest->m_r12BitReg[i] = rSource.m_r12BitReg[i];
        }
    }
    else
    {
        DEBUG_ERROR("12bitReg: copy failed due to NULL pointer");
        error = ERROR_12BIT_NULL;
    }

    return error;
}


//copies the register value from rSource to rDest
//bool* rSource holds the register value that will be coppied, sent in as rSource[12]
//st12bitReg* rDest holds the register value that will be set
//returns standard error
 int st12bitReg::copyReg(bool* rSource, st12bitReg* rDest)
 {
    int error = ERROR_NONE;

    if(rSource && rDest)
    {
        for(int i = 0; i < REG_12BIT; ++i)
        {
            if(rSource[i])
            {
                rDest->m_r12BitReg[i] = rSource[i];
            }
            else
            {
                DEBUG_ERROR("12bitReg: boolean array out of bounds");
                error = ERROR_12BIT_OPP_FAILED;
                break;
            }
        }
    }
    else
    {
        DEBUG_ERROR("12bitReg: copy failed due to NULL pointer");
        error = ERROR_12BIT_NULL;
    }

    return error;
}


//sets the register value
//st12bitReg rInReg holds the register value that will be set
//returns standard error
int st12bitReg::setReg(st12bitReg rInReg)
{
    int error = ERROR_NONE;

    for(int i = 0; i < REG_12BIT; ++i)
    {
        m_r12BitReg[i] = rInReg.m_r12BitReg[i];
    }

    return error;
}


//sets the register value
//bool* rInReg should be sent is as bool rInReg[12]
//returns standard error
int st12bitReg::setReg(bool* rInReg)
{
    int error = ERROR_NONE;

    if(rInReg)
    {
        for(int i = 0; i < REG_12BIT; ++i)
        {
            if(rInReg[i])
            {
                m_r12BitReg[i] = rInReg[i];
            }
            else
            {
                DEBUG_ERROR("12bitReg: boolean array out of bounds");
                error = ERROR_12BIT_OPP_FAILED;
                break;
            }
        }
    }
    else
    {
        DEBUG_ERROR("12bitReg: set failed due to NULL pointer");
        error = ERROR_12BIT_NULL;
    }

    return error;
}


//retrieves the register value
//rOutReg should be sent in as NULL, will return with register value
//returns standard error
 int st12bitReg::getReg(st12bitReg* rOutReg)
{
    int error = ERROR_NONE;

    //return the address of a temp, so the original register isn't changed
    st12bitReg temp;

    error = copyReg(m_r12BitReg, &temp);

    if(error = ERROR_NONE)
    {
        //delete if previously allocated
        if(rOutReg)
        {
            delete rOutReg;
        }

        rOutReg = &temp;
    }

    return error;
}


 //sets all 12 bits of the register to 0
void st12bitReg::clearReg()
{
    for(int i = 0; i < REG_12BIT; ++i)
    {
        m_r12BitReg[i] = 0;
    }
}


//returns the length of the boolean register
int st12bitReg::getLength(bool* rInReg)
{
    int count = 0;

    while(rInReg)
    {
        ++count;
        ++rInReg;
    }

    return count;
}


//copies the register value from rSource to rDest
//bool* rSource holds the register value that will be coppied, sent in as rSource[4]
//st12bitReg* rDest holds the register value that will be set
//returns standard error
int st12bitReg::get4Bits(bool* rSource, bool* rDest, int iBegin, int iEnd)
{
    int error = ERROR_NONE;
    int iRange = iEnd - iBegin;
    int iSourceLength = getLength(rSource);

    if(rDest)
    {
        if((HEX_4BIT == iRange) && (iBegin > 0) && (iEnd < REG_12BIT) && (iSourceLength == HEX_4BIT))
        {
            for(int i = iBegin; i < iEnd; ++i)
            {
                if(rDest[i])
                {
                    rDest[i] = rSource[i];
                }
                else
                {
                    DEBUG_ERROR("12bitReg: get failed due to NULL pointer");
                    error = ERROR_12BIT_NULL;
                    break;
                }
            }
        }
        else
        {
            DEBUG_ERROR("12bitReg: 4 bit copy is out of range");
            error = ERROR_12BIT_OUT_OF_RANGE;
        }
    }
    else
    {
        DEBUG_ERROR("12bitReg: get failed due to NULL pointer");
        error = ERROR_12BIT_NULL;
    }

    return error;
}


//returns the hex value of the register
int st12bitReg::getHex(unsigned int* rOutReg)
{
    int error = ERROR_NONE;
    bool inReg[HEX_4BIT];
    unsigned int reg1 = 0;
    unsigned int reg2 = 0;
    unsigned int reg3 = 0;
    int iShift = 4;

    error = get4Bits(m_r12BitReg, inReg, 0, 3);

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToNumber(inReg, &reg1);
    }

    if(error = ERROR_NONE)
    {
        error = get4Bits(m_r12BitReg, inReg, 4, 7);
    }

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToNumber(inReg, &reg2);
    }

    if(error = ERROR_NONE)
    {
        error = get4Bits(m_r12BitReg, inReg, 8, 11);
    }

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToNumber(inReg, &reg3);
    }

    if(error = ERROR_NONE)
    {
        *rOutReg = 0;
        *rOutReg |= reg1;
        reg2 << iShift;
        *rOutReg |= reg2;
        reg3 << (iShift*2);
        *rOutReg |= reg3;
    }

    return error;
}


//returns the string value of the register
int st12bitReg::getString(char* rOutReg)
{
    int error = ERROR_NONE;
    bool inReg[HEX_4BIT];
    char reg1 = '0';
    char reg2 = '0';
    char reg3 = '0';

    error = get4Bits(m_r12BitReg, inReg, 0, 3);

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToString(inReg, &reg1);
    }

    if(error = ERROR_NONE)
    {
        error = get4Bits(m_r12BitReg, inReg, 4, 7);
    }

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToString(inReg, &reg2);
    }

    if(error = ERROR_NONE)
    {
        error = get4Bits(m_r12BitReg, inReg, 8, 11);
    }

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToString(inReg, &reg3);
    }

    if(error = ERROR_NONE)
    {
        strcat(rOutReg, &reg3);
        strcat(rOutReg, &reg2);
        strcat(rOutReg, &reg1);
    }

    return error;
}


//sets the register with a hex value
int st12bitReg::setHex(unsigned int rOutReg)
{
    int error = ERROR_NONE;
    bool bInReg0[HEX_4BIT];
    bool bInReg1[HEX_4BIT];
    bool bInReg2[HEX_4BIT];
    bool bOutReg[REG_12BIT];
    unsigned int reg0 = 0;
    unsigned int reg1 = 0;
    unsigned int reg2 = 0;

    //break into 4 bit chunks
    reg0 = rOutReg & MASK0;
    reg1 = rOutReg & MASK1;
    reg2 = rOutReg & MASK2;

    error = m_HexTable.convToBinary(reg0, bInReg0);

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToBinary(reg1, bInReg1);
    }

    if(error = ERROR_NONE)
    {
        error = m_HexTable.convToBinary(reg2, bInReg2);
    }

    if(error == ERROR_NONE)
    {
        error = setBinary(bInReg0, bInReg1, bInReg2, bOutReg);
    }

    if(error == ERROR_NONE)
    {
        error = setReg(bOutReg);
    }

    return error;
}


//sets the register with a string value
//rOutReg is sent in as rOutReg[3]
int st12bitReg::setString(char* rOutReg)
{
    int error = ERROR_NONE;
    bool bInReg0[HEX_4BIT];
    bool bInReg1[HEX_4BIT];
    bool bInReg2[HEX_4BIT];
    bool bOutReg[REG_12BIT];
    char reg0 = '0';
    char reg1 = '0';
    char reg2 = '0';

    if(rOutReg)
    {
        if(rOutReg[0])
        {
            reg0 = rOutReg[0];
            if(rOutReg[1])
            {
                reg1 = rOutReg[1];
                if(rOutReg[2])
                {
                    reg2 = rOutReg[2];
                }
                else
                {
                    DEBUG_ERROR("12bitReg: set failed due to NULL pointer");
                    error = ERROR_12BIT_NULL;
                }
            }
            else
            {
                DEBUG_ERROR("12bitReg: set failed due to NULL pointer");
                error = ERROR_12BIT_NULL;
            }
        }
        else
        {
            DEBUG_ERROR("12bitReg: set failed due to NULL pointer");
            error = ERROR_12BIT_NULL;
        }

        if(error == ERROR_NONE)
        {
            error = m_HexTable.convToBinary(reg0, bInReg0);
        }

        if(error == ERROR_NONE)
        {
            error = m_HexTable.convToBinary(reg1, bInReg1);
        }

        if(error == ERROR_NONE)
        {
            error = m_HexTable.convToBinary(reg2, bInReg2);
        }

        if(error == ERROR_NONE)
        {
            error = setBinary(bInReg0, bInReg1, bInReg2, bOutReg);
        }

        if(error == ERROR_NONE)
        {
            error = setReg(bOutReg);
        }
    }
    else
    {
        DEBUG_ERROR("12bitReg: set failed due to NULL pointer");
        error = ERROR_12BIT_NULL;
    }


    return error;
}


//converts 3 bool[4] to one bool[12]
//inputs come in as bool[4], rIn0 is LSB, rIn2 is MSB
//output comes in as bool[12]
//returns standard error
int st12bitReg::setBinary(bool* rIn0, bool* rIn1, bool* rIn2, bool* rOut)
{
    int error = ERROR_NONE;
    int iIndex = 0;
    int iLength = 0;

    iLength = getLength(rIn0);

    if(HEX_4BIT == iLength)
    {
        iLength = getLength(rIn1);

        if(HEX_4BIT == iLength)
        {
            iLength = getLength(rIn2);

            if(HEX_4BIT == iLength)
            {
                for(int i = 0; i < HEX_4BIT; ++i)
                {
                    rOut[iIndex] = rIn0[i];
                    ++iIndex;
                }

                for(int i = 0; i < HEX_4BIT; ++i)
                {
                    rOut[iIndex] = rIn1[i];
                    ++iIndex;
                }

                for(int i = 0; i < HEX_4BIT; ++i)
                {
                    rOut[iIndex] = rIn2[i];
                    ++iIndex;
                }

            }
            else
            {
                DEBUG_ERROR("12bitReg: binary length is out of range");
                error = ERROR_12BIT_OUT_OF_RANGE;
            }
        }
        else
        {
            DEBUG_ERROR("12bitReg: binary length is out of range");
            error = ERROR_12BIT_OUT_OF_RANGE;
        }
    }
    else
    {
        DEBUG_ERROR("12bitReg: binary length is out of range");
        error = ERROR_12BIT_OUT_OF_RANGE;
    }

    return error;
}

