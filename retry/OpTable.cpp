/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    OpTable.cpp
Date:			    03/02/2015
Description:	 This file contains the classes Opcode7inst, OpRow and
                optable
================================================================================== */

#include <cstdio>
#include <cstring>
#include "Common.h"
#include "alu.h"
#include "OpTable.h"

//External objects
//================================================================================== 
extern ErrorTable Error;


//================================================================================== 
//Description: constructor
//================================================================================== 
OpRow::OpRow()
{
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
    m_sMnemonic = NULL;
    m_iCycles = 0;
}


//================================================================================== 
//Description: initializes an opcode row with it's name, number of cycles and 
//  opcode index
//==================================================================================
OpRow::OpRow(const char* mnem, unsigned short cycles, unsigned short index)
{
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
    if(mnem)
    {
        int strsize = strlen(mnem)+1;
        m_sMnemonic = new char[strsize];
        strcpy_s(m_sMnemonic, strsize, mnem);
    }
    else
    {
        m_sMnemonic = NULL;
    }
    m_iCycles = cycles;
    m_rOpcode = index;
}


//================================================================================== 
//Description: deconstructor
//================================================================================== 
OpRow::~OpRow()
{
    clearOpRow();
}


//==================================================================================
//Description: clears all fields and deletes the string name
//================================================================================== 
void OpRow::clearOpRow()
{
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;

    if(m_sMnemonic)
    {
        delete[] m_sMnemonic;
        m_sMnemonic = NULL;
    }

    m_iCycles = 0;
    m_rOpcode = 0;
}


//================================================================================== 
//Description: returns the name of the opcode
//================================================================================== 
char* OpRow::getName()
{
    char* mnem = NULL;
    int strsize = strlen(m_sMnemonic)+1;
    mnem = new char[strsize];
    strcpy_s(mnem, strsize, m_sMnemonic);
    return mnem;
}


//================================================================================== 
//Description: returns the opcode id
//==================================================================================
unsigned short OpRow::getOpcode()
{
    return m_rOpcode;
}


//================================================================================== 
//Description: returns the number of cycles necessary to run this instruction
//================================================================================== 
unsigned short OpRow::getCycles()
{
    return m_iCycles;
}


//================================================================================== 
//Description: returns the total number of cycles ran for this instruction
//================================================================================== 
unsigned short OpRow::getTotalCycles()
{
    return m_iTotalCycles;
}


//================================================================================== 
//Description: returns the total number of times this instruction has run
//================================================================================== 
unsigned short OpRow::getTotalUsage()
{
    return m_iTotalUsed;
}


//================================================================================== 
//Description: increments the cycle count by one
//================================================================================== 
void OpRow::incOneCycle()
{
    ++m_iTotalCycles;
}


//================================================================================== 
//Description: increments the cycles by number of cycles necessary to run 
//  the instruction
//================================================================================== 
unsigned short OpRow::incOpsCycle()
{
    m_iTotalCycles += m_iCycles;
    ++m_iTotalUsed;

    return m_iCycles;
}


//================================================================================== 
//Description: resets a row's name, cycles to run and opcode id 
//==================================================================================
void OpRow::setRow(const char* mnem, unsigned short cycles, unsigned short index)
{
    m_iTotalCycles = 0;
    if(m_sMnemonic)
    {
        delete[] m_sMnemonic;
        m_sMnemonic = NULL;
    }
    if(mnem)
    {
        int strsize = strlen(mnem)+1;
        m_sMnemonic = new char[strsize];
        strcpy_s(m_sMnemonic, strsize, mnem);
    }
    else
    {
        m_sMnemonic = NULL;
    }
    m_iCycles = cycles;
    m_rOpcode = index;
}


//================================================================================== 
//Description: prints the name, total times run and total cycles run 
//================================================================================== 
void OpRow::printRowStats()
{
    fprintf(stdout, "%s:  executed: %d times  consumed: %d cycles\n",
            m_sMnemonic, m_iTotalUsed, m_iTotalCycles);
}


//================================================================================== 
//Description: constructor
//================================================================================== 
optable::optable()
{
    const char* temp1 = OPSTRING_AND;
    const char* temp2 = OPSTRING_TAD;
    const char* temp3 = OPSTRING_ISZ;
    const char* temp4 = OPSTRING_DCA;
    const char* temp5 = OPSTRING_JMS;
    const char* temp6 = OPSTRING_JMP;
    const char* temp7 = OPSTRING_IO;
    const char* temp8 = OPSTRING_MICRO;

    m_opTable[0].setRow(temp1, 2, 0);
    m_opTable[1].setRow(temp2, 2, 1);
    m_opTable[2].setRow(temp3, 2, 2);
    m_opTable[3].setRow(temp4, 2, 3);
    m_opTable[4].setRow(temp5, 2, 4);
    m_opTable[5].setRow(temp6, 1, 5);
    m_opTable[6].setRow(temp7, 0, 6);
    m_opTable[7].setRow(temp8, 1, 7);

    m_alu = NULL;
}


//================================================================================== 
//Description: opcode constructor with accumulator pointer
//================================================================================== 
optable::optable(alu* mem)
{
    const char* temp1 = OPSTRING_AND;
    const char* temp2 = OPSTRING_TAD;
    const char* temp3 = OPSTRING_ISZ;
    const char* temp4 = OPSTRING_DCA;
    const char* temp5 = OPSTRING_JMS;
    const char* temp6 = OPSTRING_JMP;
    const char* temp7 = OPSTRING_IO;
    const char* temp8 = OPSTRING_MICRO;

    m_opTable[0].setRow(temp1, 2, 0);
    m_opTable[1].setRow(temp2, 2, 1);
    m_opTable[2].setRow(temp3, 2, 2);
    m_opTable[3].setRow(temp4, 2, 3);
    m_opTable[4].setRow(temp5, 2, 4);
    m_opTable[5].setRow(temp6, 1, 5);
    m_opTable[6].setRow(temp7, 0, 6);
    m_opTable[7].setRow(temp8, 1, 7);

    if(mem)
    {
        m_alu = mem;
    }
    else
    {
        m_alu = NULL;
    }
}


//================================================================================== 
//Description: deconstructor
//================================================================================== 
optable::~optable()
{
    if(m_alu)
    {
        delete m_alu;
        m_alu = NULL;
    }
}


//================================================================================== 
//Description: finds the id of the opcode from it's name
//================================================================================== 
unsigned short optable::getIndex(char* name)
{
    unsigned short iFoundIndex = OP_TABLE_LENG;
    char* tablename = NULL;

    if(name)
    {
        for(int i = 0; i < OP_TABLE_LENG; ++i)
        {
            tablename = m_opTable[i].getName();
            if(tablename)
            {
                if(!strcmp(tablename, name))
                {
                    iFoundIndex = i;
                    break;
                }

                delete[] tablename;
                tablename = NULL;
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_OPTABLE);
            }
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_OPTABLE);
    }
    
    if(tablename)
    {
        delete[] tablename;
        tablename = NULL;
    }
    

    return iFoundIndex;
}


//================================================================================== 
//Description: finds the name of an instruction from its id
//==================================================================================
char* optable::getMnemonic(unsigned short index)
{
    char* temp = NULL;

    if(index < OP_TABLE_LENG)
    {
        temp = m_opTable[index].getName();
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
    }

    return temp;
}


//================================================================================== 
//Description: returns the total number of cycles for the input id
//================================================================================== 
unsigned short optable::getTotalOpsCycles(unsigned short index)
{
    unsigned short temp = 0;

    if(index < OP_TABLE_LENG)
    {
        temp = m_opTable[index].getTotalCycles();
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
    }

    return temp;
}


//================================================================================== 
//Description: returns the total number of cycles run
//================================================================================== 
unsigned short optable::getTotalCycles()
{
    return m_totalCycles;
}


//==================================================================================
//Description: returns the total number of times the input instruction has been used
//================================================================================== 
unsigned short optable::getTotalUsed(char* name)
{
    unsigned short index = 0;
    unsigned short used = 0;

    if(name)
    {
        index = getIndex(name);

        if(index < OP_TABLE_LENG)
        {
            used = m_opTable[index].getTotalUsage();
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_OPTABLE);
    }
    
    return used;
}


//================================================================================== 
//Description: increase the number of cycles for the inputs id by it's default 
//================================================================================== 
void optable::incrementOpsCycle(unsigned short index)
{
    unsigned short count = 0;

    if(index < OP_TABLE_LENG)
    {
        count = m_opTable[index].incOpsCycle();
        m_totalCycles += count;
        ++m_totalInstructions;
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
    }

}


//================================================================================== 
//Description: increments the opcode id by one cycle
//================================================================================== 
void optable::incrementOneCycle(unsigned short index)
{
    if(index < OP_TABLE_LENG)
    {
        m_opTable[index].incOneCycle();
        ++m_totalCycles;
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
    }
}


//================================================================================== 
//Description: prints the total usage of all instructions
//================================================================================== 
void optable::printUsageAndCycles()
{
    fprintf(stdout, "\n%s\nStatistic Summary\n%s\n", PRINT_BREAK, PRINT_BREAK);
    fprintf(stdout, "Total instructions: %d   Total clock cycles: %d \n%s\n", 
            m_totalInstructions, m_totalCycles, PRINT_BREAK);

    for(int i = 0; i < OP_TABLE_LENG; ++i)
    {
        m_opTable[i].printRowStats();
    }
}


