#include "stBitReg.h"
#include "Common.h"
#include "stOctConv.h"
#include <cstdio>
#include <cstring>
#include <cmath>


//constructor
stBitReg::stBitReg()
{
    m_rBitReg = NULL;
    m_iLength = 0;
    
    clearReg();

}


//constructor for empty register
stBitReg::stBitReg(int regSize)
{
    int error = ERROR_NONE;


    if(regSize > 0)
    {
        m_iLength = regSize;
        m_rBitReg = new bool[m_iLength];
        clearReg();
    }
    else
    {
        stBitReg();
        error = ERROR_BIT_OUT_OF_RANGE;
    }

    if(error != ERROR_NONE)
    {
        DEBUG_ERROR("bitReg: constructor initialization failed");
        DEBUG_CODE(error);
    }
}


//constructor with bool
stBitReg::stBitReg(bool* rSource, int regSize)
{
    int error = ERROR_NONE;

    m_iLength = regSize;

    stBitReg rInReg = setReg(rSource);

    error = setReg(rSource);

    if(error != ERROR_NONE)
    {
        DEBUG_ERROR("bitReg: constructor initialization failed");
    }
}


//constructor with char
stBitReg::stBitReg(char* rSource, int regSize)
{
    int error = ERROR_NONE;

    m_iLength = regSize;

    error = setReg(rSource);

    if(error != ERROR_NONE)
    {
        DEBUG_ERROR("bitReg: constructor initialization failed");
    }
}


//constructor with int
stBitReg::stBitReg(unsigned int rSource, int regSize)
{
    int error = ERROR_NONE;

    m_iLength = regSize;

    error = setReg(rSource);

    if(error != ERROR_NONE)
    {
        DEBUG_ERROR("bitReg: constructor initialization failed");
    }
}


//deconstructor
stBitReg::~stBitReg()
{
    clearReg();

    if(m_rBitReg)
    {
        delete [] m_rBitReg;
    }

    m_rBitReg = NULL;
    m_iLength = 0;
}


//copies the register value from rSource to stBitReg
//stBitReg rSource holds the register value that will be coppied
//returns stBitReg with coppied value
stBitReg stBitReg::copyReg(stBitReg rSource)
{
    int iLength = getLength(rSource);

    stBitReg rDest(rSource, iLength);

    return rDest;
}


//copies the register value from rSource to stBitReg
//bool* rSource holds the register value that will be coppied
//returns stBitReg with coppied value
stBitReg stBitReg::copyReg(bool* rSource)
{
    int iLength = getLength(rSource);

    stBitReg rDest(rSource, iLength);

    return rDest;
 }


//copies the register value from rSource to stBitReg
//bool* rSource holds the register value that will be coppied
//returns stBitReg with coppied value
stBitReg stBitReg::copyReg(char* rSource)
{
    int iLength = getLength(rSource);

    stBitReg rDest(rSource, iLength);

    return rDest;
}


//copies the register value from rSource to stBitReg
//unsigned int rSource holds the register value that will be coppied
//returns stBitReg with coppied value
stBitReg stBitReg::copyReg(unsigned int rSource)
{
    int iLength = getLength(rSource);

    stBitReg rDest(rSource, iLength);

    return rDest;
}


//saves to the register 3 bits starting at index and decreasing
int stBitReg::save3bit(bool* bReg, int index)
{
    if(bReg)
    {
        for(int i = 0; i > OCT_3BIT; ++i)
        {
            if(bReg[i])
            {
                m_rBitReg[index] = bReg[i];
                --index;
            }
            else
            {
                break;
            }
        }
    }
    return index;
}


 //sets all bits of the register to 0
 void stBitReg::clearReg()
 {
     for(int i = 0; i < m_iLength; ++i)
     {
         m_rBitReg[i] = 0;
     }
 }


 //returns the length of the boolean register
 int stBitReg::getLength(bool* rInReg)
 {
     int count = 0;

     while(rInReg)
     {
         ++count;
         ++rInReg;
     }

     return count;
 }


 //returns the length of the boolean register
 int stBitReg::getLength()
 {
     return m_iLength;
 }


 //returns the length of the boolean register
 int stBitReg::getLength(stBitReg rInReg)
 {
     return rInReg.m_iLength;
 }


 //returns the length of the boolean register
 int stBitReg::getLength(bool* rInReg)
 {
     int count = sizeof(rInReg)/sizeof(bool);

     return count;
 }


 //returns the length of the char register (a char is 3 bits)
 int stBitReg::getLength(char* rInReg)
 {
     int count = sizeof(rInReg)/sizeof(char); //number of chars
     count *= 3; //convert to bits

     return count;
 }


 //returns the length of the unsigned int, where int is in decimal
 int stBitReg::getLength(unsigned int rInReg)
 {
     int count = 0;
     int remainder = rInReg;
     
     //find number of bits
     while(remainder)
     {
         ++count;
         remainder /= 2;
     }

     return count;
 }


//sets the register value
//stBitReg rInReg holds the register value that will be set
//returns standard error
int stBitReg::setReg(stBitReg rInReg)
{
    int error = ERROR_NONE;

    if(rInReg.m_rBitReg)
    {
        for(int i = 0; i < m_iLength; ++i)
        {
            if(rInReg.m_rBitReg[i])
            {
                m_rBitReg[i] = rInReg.m_rBitReg[i];
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        DEBUG_ERROR("bitReg: boolean array out of bounds");
        error = ERROR_BIT_OPP_FAILED;
    }

    return error;
}


//sets the register value
//bool* rInReg should be sent is as bool rInReg[12]
//returns standard error
int stBitReg::setReg(bool* rInReg)
{
    int error = ERROR_NONE;

    if(rInReg)
    {
        for(int i = 0; i < m_iLength; ++i)
        {
            if(rInReg[i])
            {
                m_rBitReg[i] = rInReg[i];
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        DEBUG_ERROR("bitReg: set failed due to NULL pointer");
        error = ERROR_BIT_NULL;
    }

    return error;
}


int stBitReg::setReg(char* rInReg)
{
    int error = ERROR_NONE;
    int index = m_iLength-1;
    int iLength = getLength(rInReg);
    bool* bOutReg = NULL;

    iLength /= 3; //convert to number of chars in string

    if(rInReg)
    {
        for(int i = 0; i < m_iLength; i += 3) //make the incoming string isn't bigger than the register
        {
            --iLength;  //go to next lowest char

            if(iLength > -1) 
            {
                bOutReg = OctTable.convToBinary(rInReg[iLength]);  //get the 3 bit  equivalent of char
                index = save3bit(bOutReg, index);
            }
            else //if the incomming string is shorter than the register
            {
                break;
            }
        }
    }
    else
    {
        DEBUG_ERROR("bitReg: boolean array out of bounds");
        error = ERROR_BIT_OPP_FAILED;
    }

    return error;
}


int stBitReg::setReg(unsigned int rInReg)
{
    int error = ERROR_NONE;
    int index = m_iLength-1;
    int iLength = getLength(rInReg); //number of bits
    int iMax = 0; 
    int iDiv = rInReg;
    unsigned int* iModTemp = NULL; 
    bool* bOutReg = NULL;

    //get max amount of bits 3 bit converts
    if(iLength > 0)
    {
        iMax = (rInReg/8) + 1;
        iModTemp = new unsigned int[iMax];
    }
        
    for(int i = 0; i < iMax; ++i)
    {
        iModTemp[i] = iDiv%8; //get mod of 8 for each 3 bit set
        iDiv /= 8;
    }
     
    //load bits by 3 into register
    for(int j = 0; j < m_iLength; ++j)
    {
        --iMax;
        if(iMax > -1)
        {
            bOutReg = OctTable.convToBinary(iModTemp[iMax]);
            index = save3bit(bOutReg, index);
        }
        else //if the incomming string is shorter than the register
        {
            break;
        }
    }

    if(iModTemp)
    {
        delete[] iModTemp;
    }
    
}


//retrieves the register value 
//returns the register value
stBitReg stBitReg::getReg()
{
    //return temp, so the original register isn't changed
    stBitReg temp = copyReg(m_rBitReg);

    return temp;
}


bool* stBitReg::getBool()
{
    bool* bTemp = NULL;
    //return temp, so the original register isn't changed
    stBitReg temp = copyReg(m_rBitReg);

    return temp.m_rBitReg;
}


unsigned int stBitReg::getNumber()
{
    int iTotalOctBytes = m_iLength/3;
    int iCurrent = 0;
    unsigned int iTotal = 0;
    unsigned int iTemp = 0;
    stBitReg temp;

    if(m_iLength%3 != 0)
    {
        ++iTotalOctBytes;
    }

    for(int i = 0; i < iTotalOctBytes; ++i)
    {
        iCurrent = i*3;
        temp = sliceBits(iCurrent, (iCurrent+2));
        iTemp = OctTable.convToNumber(temp.m_rBitReg);
        iTemp = (iTemp*pow(8, i));
        iTotal += iTemp;
    }

    return iTotal;
}


unsigned int stBitReg::getNumber(bool* rInReg)
{
    int iLength = getLength(rInReg);
    stBitReg temp(rInReg, iLength);

    return temp.getNumber();
}


unsigned int stBitReg::getNumber(char* rInReg)
{
    int iLength = getLength(rInReg);
    int iTotalOctBytes = m_iLength/3;
    int iCurrent = 0;
    unsigned int iTotal = 0;
    unsigned int iTemp = 0;
    stBitReg temp(rInReg, iLength);

    if(m_iLength%3 != 0)
    {
        ++iTotalOctBytes;
    }

    for(int i = 0; i < iTotalOctBytes; ++i)
    {
        iTemp = OctTable.convToNumber(rInReg[i]);
        iTemp = (iTemp*pow(8, i));
        iTotal += iTemp;
    }

    return iTemp;
}

char* stBitReg::getString()
{
    int iTotalOctBytes = m_iLength/3;
    int iCurrent = 0;
    char sTemp = '0';
    char* pTemp = NULL;
    stBitReg temp;

    //add one more bit if there is a remainder
    if(m_iLength%3 != 0)
    {
        ++iTotalOctBytes;
    }

    pTemp = new char[iTotalOctBytes];

    for(int i = 0; i < iTotalOctBytes; ++i)
    {
        iCurrent = i*3;
        temp = sliceBits(iCurrent, (iCurrent+2));
        sTemp = OctTable.convToString(temp.m_rBitReg);
        pTemp[i] = sTemp;
    }

    return pTemp;
}


char* stBitReg::getString(stBitReg rInReg)
{
    int iTotalOctBytes = rInReg.m_iLength/3;
    int iCurrent = 0;
    char sTemp = '0';
    char* pTemp = NULL;
    stBitReg temp;

    //add one more bit if there is a remainder
    if(m_iLength%3 != 0)
    {
        ++iTotalOctBytes;
    }

    pTemp = new char[iTotalOctBytes];

    for(int i = 0; i < iTotalOctBytes; ++i)
    {
        iCurrent = i*3;
        temp = sliceBits(iCurrent, (iCurrent+2));
        sTemp = OctTable.convToString(temp.m_rBitReg);
        pTemp[i] = sTemp;
    }

    return pTemp;
}


char* stBitReg::getString(bool* rInReg)
{
    int iLength = getLength(rInReg);
    int iTotalOctBytes = iLength/3;
    bool temp[OCT_3BIT] = {0, 0, 0};
    int iCurrent = 0;
    char sTemp = '0';
    char* pTemp = NULL;

    //add one more bit if there is a remainder
    if(iLength%3 != 0)
    {
        ++iTotalOctBytes;
    }

    pTemp = new char[iTotalOctBytes];

    for(int i = 0; i < iTotalOctBytes; ++i)
    {
        iCurrent = i*3;
        for(int j = 0; j < (OCT_3BIT); ++j)
        {
            temp[j] = rInReg[iCurrent];
            ++iCurrent;
        }
        sTemp = OctTable.convToString(temp);
        pTemp[i] = sTemp;
    }

    return pTemp;
}


char* stBitReg::getString(unsigned int rInReg)
{
    int iLength = getLength(rInReg);
    int iTotalOctBytes = iLength/3;
    unsigned int iTemp = 0;
    char sTemp = '0';
    char* pTemp = NULL;

    //add one more bit if there is a remainder
    if(iLength%3 != 0)
    {
        ++iTotalOctBytes;
    }

    pTemp = new char[iTotalOctBytes];

    for(int i = 0; i < iTotalOctBytes; ++i)
    {
        iTemp = iTemp%8;
        pTemp[i] = OctTable.convToString(iTemp);
        iTemp /= 8;
    }

    return pTemp;
}


//copies the register value from rSource to rDest
//stBitReg rSource holds the register value that will be coppied, sent in as rSource
//int iBegin is the starting index
//int iEnd is the ending index
//returns standard error
stBitReg stBitReg::sliceBits(int iBegin, int iEnd)
{
    int iRange = iEnd - iBegin;
    int index = 0;
    stBitReg rDest(iRange);

    if((iBegin > 0) && (m_iLength > --iRange))
    {
        for(int i = iBegin; i < iEnd; ++i)
        {
            rDest.m_rBitReg[index] = m_rBitReg[i];
            ++index;
        }
    }
    else
    {
        DEBUG_ERROR("bitReg: bit copy is out of range");
    }

    return rDest;
}


//concatenates octal words
//stBitReg* rSource is an array of stBitReg
//      to be concatenated, rSource[max] = MSB, rSource[0] = LSB
//int iNumberWords are number of stBitReg being sent
//returns register of concatenated values
stBitReg stBitReg::ccatRegs(stBitReg* rSource, int iNumberWords)
{
    int index = 0;
    int iBitLength = 0;
    stBitReg rOut; //base constructor sets length to 0 and register to null

    if(rSource)
    {
        for(int k = 0; k < iNumberWords; ++k)
        {
            iBitLength += rSource[k].m_iLength;
        }

        if(iBitLength > 0)
        {
            rOut.m_iLength = iBitLength;
            rOut.m_rBitReg = new bool[iBitLength];

            --iNumberWords; //start at the largest register to place in index 0

            for(int i = iNumberWords; i > -1; --i)
            {
                for(int j = rSource[i].m_iLength; j > -1; --j) //handle registers in big endian
                {
                    rOut.m_rBitReg[index] = rSource[i].m_rBitReg[j];
                    ++index;
                }
            }
        }
        else
        {
            DEBUG_ERROR("bitReg: register length is out of range");
        }
    }
    else
    {
        DEBUG_ERROR("bitReg: source is null");
    }

    return rOut;
}


