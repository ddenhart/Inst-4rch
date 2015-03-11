/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    OpTable.cpp
Date:			    03/02/2015
Description:	 This file contains the classes Opcode7inst, OpRow and
                OpTableHandle
================================================================================== */

#include <cstdio>
#include <cstring>
#include "Common.h"
#include "BitReg.h"
#include "Accumulator.h"
#include "OpTable.h"

//External objects
//================================================================================== 
extern ErrorTable Error;
extern RegTable RegisterFile;


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Opcode7inst::Opcode7inst()
{
    m_iCode = 0;
    m_sMnemonic = NULL;
    m_rValue.setReg(REG_12BIT);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Opcode7inst::~Opcode7inst()
{
    if(m_sMnemonic)
    {
        delete[] m_sMnemonic;
        m_sMnemonic = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void Opcode7inst::setRow(unsigned int code, const char* mnem)
{
    m_iCode = code;
    if(mnem)
    {
        m_sMnemonic = new char[strlen(mnem)+1];
        strcpy(m_sMnemonic, mnem);
    }
    else
    {
        m_sMnemonic = NULL;
    }

    code += 7000;  //add in the opcode
    m_rValue.setReg(code);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int Opcode7inst::getcode()
{
    return m_iCode;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Opcode7List::Opcode7List()
{
    const char* temp1 = "CMA";
    const char* temp2 = "IAC";
    const char* temp3 = "CIA";
    const char* temp4 = "CLA CLL";
    const char* temp5 = "HLT";
    const char* temp6 = "SMA";
    const char* temp7 = "SZA";
    const char* temp8 = "SPA";
    const char* temp9 = "SNA";
    const char* temp10 = "RAR";
    const char* temp11 = "RAL";

    m_op7List[0].setRow(040, temp1); //complement accumulator
    m_op7List[1].setRow(001, temp2); //increment accumulator
    m_op7List[2].setRow(041, temp3); //complement and increment accumulator
    m_op7List[3].setRow(300, temp4); //clear AC and link
    m_op7List[4].setRow(402, temp5); //halt
    m_op7List[5].setRow(500, temp6); //skip next instruction if AC is negative
    m_op7List[6].setRow(440, temp7); //skip next instruction if AC is zero
    m_op7List[7].setRow(510, temp8); //skip next instruction if AC is positive
    m_op7List[8].setRow(450, temp9); //skip next instruction if AC is not equal to zero
    m_op7List[9].setRow(010, temp10); //rotate accumulator link pair right
    m_op7List[10].setRow(004, temp11); //rotate accumulator link par left
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Opcode7List::~Opcode7List()
{

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
int Opcode7List::findMicroOp(unsigned int inst)
{
    bool found = false;
    int index = -1;

    for(int i = 0; i < OP7_LIST_LENG; ++i)
    {
        if(inst == m_op7List[i].getcode())
        {
            found = true;
            index = i;
            break;
        }
    }

    return index;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OpRow::OpRow()
{
    m_iTotalCount = 0;
    m_sMnemonic = NULL;
    m_iCycles = 0;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
OpRow::OpRow(const char* mnem, int cycles, unsigned int index)
{
    m_iTotalCount = 0;
    m_sMnemonic = new char[strlen(mnem)+1];
    strcpy(m_sMnemonic, mnem);
    m_iCycles = cycles;
    m_rOpcode.setReg(index);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OpRow::~OpRow()
{
    clearOpRow();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpRow::clearOpRow()
{
    m_iTotalCount = 0;

    if(m_sMnemonic)
    {
        delete[] m_sMnemonic;
        m_sMnemonic = NULL;
    }

    m_iCycles = 0;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
char* OpRow::getName()
{
    char* mnem = NULL;
    
    mnem = new char[strlen(m_sMnemonic)+1];
    strcpy(mnem, m_sMnemonic);
    return mnem;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int OpRow::getOpcode()
{
    return m_rOpcode.getNumber();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int OpRow::getCycles()
{
    return m_iCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int OpRow::getTotal()
{
    return m_iTotalCount;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpRow::incTotal()
{
    ++m_iTotalCount;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void OpRow::setRow(const char* mnem, int cycles, unsigned int index)
{
    m_iTotalCount = 0;
    if(m_sMnemonic)
    {
        delete[] m_sMnemonic;
        m_sMnemonic = NULL;
    }

    m_sMnemonic = new char[strlen(mnem)+1];
    strcpy(m_sMnemonic, mnem);
    m_iCycles = cycles;
    m_rOpcode.setSize(REG_3BIT);
    m_rOpcode.setReg(index);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpRow::printRow()
{
    fprintf(stdout, "Opcode: %s   Value: %s   Cycles: %d   Total: %d\n",
            m_sMnemonic, m_rOpcode.getString(), m_iCycles, m_iTotalCount);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OpTableHandle::OpTableHandle()
{
    const char* temp1 = "AND";
    const char* temp2 = "TAD";
    const char* temp3 = "ISZ";
    const char* temp4 = "DCA";
    const char* temp5 = "JMS";
    const char* temp6 = "JMP";
    const char* temp7 = "<IO>";
    const char* temp8 = "uInstructions";

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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OpTableHandle::OpTableHandle(Accumulator* mem)
{
    const char* temp1 = "AND";
    const char* temp2 = "TAD";
    const char* temp3 = "ISZ";
    const char* temp4 = "DCA";
    const char* temp5 = "JMS";
    const char* temp6 = "JMP";
    const char* temp7 = "<IO>";
    const char* temp8 = "uInstructions";

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
        Error.printError(ERROR_NULL, FILE_OPTABLE);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OpTableHandle::~OpTableHandle()
{
    if(m_alu)
    {
        delete m_alu;
        m_alu = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int OpTableHandle::searchByName(char* name)
{
    int iFoundIndex = -1;
    
    for(int i = 0; i < OP_TABLE_LENG; ++i)
    {
        if(strcmp(m_opTable[i].getName(),name))
        {
            iFoundIndex = i;
            break;
        }
    }

    return iFoundIndex;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
char* OpTableHandle::getMnemonic(unsigned int index)
{
    char* temp = NULL;

    if(index < OP_TABLE_LENG)
    {
        temp = m_opTable[index].getName();
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int OpTableHandle::getIndex(char* name)
{
    int temp = -1;

    if(name)
    {
        temp = searchByName(name);
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int OpTableHandle::getTotal(int index)
{
    int temp = 0;

    if(index < OP_TABLE_LENG)
    {
        temp = m_opTable[index].getTotal();
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::incrementTotal(int index)
{
    if((index < OP_TABLE_LENG) && (index >= 0))
    {
        m_opTable[index].incTotal();
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::printTable()
{
    for(int i = 0; i < OP_TABLE_LENG; ++i)
    {
        fprintf(stdout, "%s\n     OpCode Table\n%s", PRINT_BREAK, PRINT_BREAK);
        m_opTable[i].printRow();
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::exMicroInst(int opcode)
{
    //int index = m_op7.findMicroOp(opcode);

    if(opcode == 300)
    {
        m_alu->clear();
    }
}
