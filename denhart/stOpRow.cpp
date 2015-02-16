#include "Common.h"
#include "stOctConv.h"
#include "stBitReg.h"
#include "stOpRow.h"
#include <cstdio>


 
stOpRow::stOpRow()
{
    clearOpRow();
}


stOpRow::stOpRow(int total, char* mnem, int cycles, unsigned int index)
{
    m_iTotalCount = total;
    m_sMnemonic = mnem;
    m_iCycles = cycles;
    m_rOpcode.setReg(index);
}


stOpRow::~stOpRow()
{
    clearOpRow();                                                                                   
}

void stOpRow::clearOpRow()
{
    m_iTotalCount = 0;
    
    if (m_sMnemonic)
    {
        delete [] m_sMnemonic;
    }
    
    m_sMnemonic = NULL;
    m_iCycles = 0;
    m_rOpcode.clearReg();
}


char* stOpRow::getName()
{
    return m_sMnemonic;
}


int stOpRow::getOpcode()
{
    return m_rOpcode.getNumber();
}


int stOpRow::getCycles()
{
    return m_iCycles;
}


int stOpRow::getTotal()
{
    return m_iTotalCount;
}