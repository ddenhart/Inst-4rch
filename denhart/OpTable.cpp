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
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
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
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
    m_iCode = 0;

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
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
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

    m_rValue.setReg(code);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
char* Opcode7inst::getname()
{
    return m_sMnemonic;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int Opcode7inst::getTotalCycles()
{
    return m_iTotalCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int Opcode7inst::getTotalUsed()
{
    return m_iTotalUsed;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void Opcode7inst::incCycles()
{
    ++m_iTotalCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void Opcode7inst::incUsed()
{
    ++m_iTotalUsed;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void Opcode7inst::printRowStats()
{
    fprintf(stdout, "%s:  executed: %d times  consumed: %d cycles\n",
            m_sMnemonic, m_iTotalUsed, m_iTotalCycles);
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
    const char* temp0 = OPSTRING7_NOP; //7000 no bits
    const char* temp1 = OPSTRING7_IAC; //7001 bit 11 
    const char* temp2 = OPSTRING7_BSW; //7002 bit 10
    const char* temp3 = OPSTRING7_RAL; //7004 bit 9
    const char* temp4 = OPSTRING7_RTL; //7006 bit 9 and bit 10
    const char* temp5 = OPSTRING7_RAR; //7010 bit 8
    const char* temp6 = OPSTRING7_RTR; //7012 bit 8 and bit 10
    const char* temp7 = OPSTRING7_CML; //7020 bit 7
    const char* temp8 = OPSTRING7_CMA; //7040 bit 6
    const char* temp9 = OPSTRING7_CIA; //7041 bit 6 and bit 11
    const char* temp10 = OPSTRING7_CLL; //7100 bit 5
    const char* temp11 = OPSTRING7_STL; //7120 bit 5 and bit 7
    const char* temp12 = OPSTRING7_STA; //7240 bit 4 and bit 6
    const char* temp13 = OPSTRING7_HLT; //7402 bit 10
    const char* temp14 = OPSTRING7_OSR; //7404 bit 3 and bit 9
    const char* temp15 = OPSTRING7_SKP; //7410 bit 3 and bit 8
    const char* temp16 = OPSTRING7_SNL; //7420 bit 3 and bit 7
    const char* temp17 = OPSTRING7_SZL; //7430 bit 3 and bit 7 and bit 8
    const char* temp18 = OPSTRING7_SZA; //7440 bit 3 and bit 6
    const char* temp19 = OPSTRING7_SNA; //7450 bit 3 and bit 6 and bit 8
    const char* temp20 = OPSTRING7_SMA; //7500 bit 3 and bit 5
    const char* temp21 = OPSTRING7_SPA; //7510 bit 3 and bit 5 and bit 8
    const char* temp22 = OPSTRING7_CLA; //7600 bit 3 and bit 4 or 7200 bit 4
    //7060 CMA CML  //complement the alu and link
    //7300 CLA CLL //clear the alu and link
    //CIA = CMA IAC //compliment and increment the alu 2's comp
    //STL = CLL CML //set link to 1
    //7201 CLA IAC //set alu to 1
    //STA = CLA CMA //set alu to -1

    m_op7List[0].setRow(7000, temp0); //complement accumulator
    m_op7List[1].setRow(7001, temp1); //increment accumulator
    m_op7List[2].setRow(7002, temp2); //complement and increment accumulator
    m_op7List[3].setRow(7004, temp3); //clear AC and link
    m_op7List[4].setRow(7006, temp4); //halt
    m_op7List[5].setRow(7010, temp5); //skip next instruction if AC is negative
    m_op7List[6].setRow(7012, temp6); //skip next instruction if AC is zero
    m_op7List[7].setRow(7020, temp7); //skip next instruction if AC is positive
    m_op7List[8].setRow(7040, temp8); //skip next instruction if AC is not equal to zero
    m_op7List[9].setRow(7041, temp9); //rotate accumulator link pair right
    m_op7List[10].setRow(7100, temp10); //rotate accumulator link par left
    m_op7List[11].setRow(7120, temp11);
    m_op7List[12].setRow(7240, temp12);
    m_op7List[13].setRow(7402, temp13);
    m_op7List[14].setRow(7404, temp14);
    m_op7List[15].setRow(7410, temp15);
    m_op7List[16].setRow(7420, temp16);
    m_op7List[17].setRow(7430, temp17);
    m_op7List[18].setRow(7440, temp18);
    m_op7List[19].setRow(7450, temp19);
    m_op7List[20].setRow(7500, temp20);
    m_op7List[21].setRow(7510, temp21);
    m_op7List[22].setRow(7600, temp22);

    m_TotalUsed = 0;
    m_iTotalCycles = 0;
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
    m_TotalUsed = 0;
    m_iTotalCycles = 0;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int Opcode7List::getTotalCycles()
{
    return m_iTotalCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int Opcode7List::getTotalUsed()
{
    return m_TotalUsed;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
unsigned int Opcode7List::findMicroOp(unsigned int inst)
{
    unsigned int index = OP7_LIST_LENG;
    char* name = NULL;

    for(int i = 0; i < OP7_LIST_LENG; ++i)
    {
        if(inst == m_op7List[i].getcode())
        {
            index = i;
            break;
        }
    }

    if(index == OP7_LIST_LENG)
    {
        //special cases
        if(inst == OP7_CMA_CML)
        {
            name = OPSTRING7_CML;
            index = findIndex(name);
            if(index < OP7_LIST_LENG)
            {
                //m_op7List[index].incCycles();
                m_op7List[index].incUsed();
            }
            else
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
            }
            name = OPSTRING7_CMA;
            index = findIndex(name);
        }
        else if(inst == OP7_CLA_CLL)
        {
            name = OPSTRING7_CLL;
            index = findIndex(name);
            if(index < OP7_LIST_LENG)
            {
                //m_op7List[index].incCycles();
                m_op7List[index].incUsed();
            }
            else
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
            }
            name = OPSTRING7_CLA;
            index = findIndex(name);
        }
        else if(inst == OP7_CLA_IAC)
        {
            name = OPSTRING7_IAC;
            index = findIndex(name);
            if(index < OP7_LIST_LENG)
            {
                //m_op7List[index].incCycles();
                m_op7List[index].incUsed();
            }
            else
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
            }
            name = OPSTRING7_CLA;
            index = findIndex(name);
        }
        else
        {
            nop();
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
unsigned int Opcode7List::findIndex(char* name)
{
    bool found = false;
    unsigned int index = OP7_LIST_LENG;

    if(name)
    {
        for(int i = 0; i < OP7_LIST_LENG; ++i)
        {
            if(!strcmp(name, m_op7List[i].getname()))
            {
                found = true;
                index = i;
                break;
            }
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_OPTABLE);
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
void Opcode7List::updateStats(unsigned int inst)
{
    unsigned int index = findMicroOp(inst);

    if(index < OP7_LIST_LENG)
    {
        m_op7List[index].incCycles();
        m_op7List[index].incUsed();
        ++m_TotalUsed;
        ++m_iTotalCycles;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void Opcode7List::print7Stats()
{
    fprintf(stdout, "%s\nMicro Instructions\n%s\n", PRINT_BREAK, PRINT_BREAK);

    for(int i = 0; i < OP7_LIST_LENG; ++i)
    {
        m_op7List[i].printRowStats();
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void Opcode7List::nop()
{
    fprintf(stdout, "Warning: NOP\n");
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
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
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
OpRow::OpRow(const char* mnem, unsigned int cycles, unsigned int index)
{
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;
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
    m_iTotalCycles = 0;
    m_iTotalUsed = 0;

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
unsigned int OpRow::getCycles()
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
unsigned int OpRow::getTotal()
{
    return m_iTotalCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
unsigned int OpRow::getUsage()
{
    return m_iTotalUsed;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpRow::incOne()
{
    ++m_iTotalCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
unsigned int OpRow::incOps()
{
    m_iTotalCycles += m_iCycles;
    ++m_iTotalUsed;

    return m_iCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void OpRow::setRow(const char* mnem, unsigned int cycles, unsigned int index)
{
    m_iTotalCycles = 0;
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
void OpRow::printRowStats()
{
    fprintf(stdout, "%s:  executed: %d times  consumed: %d cycles\n",
            m_sMnemonic, m_iTotalUsed, m_iTotalCycles);
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
            m_sMnemonic, m_rOpcode.getString(), m_iCycles, m_iTotalCycles);
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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OpTableHandle::OpTableHandle(Accumulator* mem)
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
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
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
unsigned int OpTableHandle::getIndex(char* name)
{
    unsigned int temp = OP_TABLE_LENG; 

    if(name)
    {
        temp = searchByName(name);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_OPTABLE);
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
unsigned int OpTableHandle::getTotalOpsCycles(unsigned int index)
{
    unsigned int temp = 0;

    if(index < OP_TABLE_LENG)
    {
        temp = m_opTable[index].getTotal();
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
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
unsigned int OpTableHandle::getTotalCycles()
{
    return m_totalCycles;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
unsigned int OpTableHandle::getTotalUsed(char* name)
{
    unsigned int index = 0;
    unsigned int used = 0; 

    if(name)
    {
        index = getIndex(name);

        if(index < OP_TABLE_LENG)
        {
            used = m_opTable[index].getUsage();
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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::incrementOpsCycle(unsigned int index)
{
    unsigned int count = 0;

    if(index < OP_TABLE_LENG)
    {
        count = m_opTable[index].incOps();

        if(index <= OPCODE_MRI)
        {
            m_totalCycles += count;
        }

        if(index < OPCODE_OPP)
        {
            incrementInstructions();
        }
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
    }

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::incrementOneCycle(unsigned int index)
{
    if((index < OP_TABLE_LENG) && (index >= 0))
    {
        m_opTable[index].incOne();
        ++m_totalCycles;
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_OPTABLE);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::incrementInstructions()
{
    ++m_totalInstructions;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void OpTableHandle::printUsageAndCycles()
{
    int max = OP_TABLE_LENG-1;

    m_totalCycles += m_op7.getTotalCycles();
    m_totalInstructions += m_op7.getTotalUsed();
    fprintf(stdout, "\n%s\nStatistic Summary\n%s\n", PRINT_BREAK, PRINT_BREAK);
    fprintf(stdout, "Total instructions: %d   Total clock cycles: %d \n%s\n", 
            m_totalInstructions, m_totalCycles, PRINT_BREAK);
    for(int i = 0; i < max; ++i)
    {
        m_opTable[i].printRowStats();
    }
    m_op7.print7Stats();
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
void OpTableHandle::updateMicros(unsigned int opcode)
{
    m_op7.updateStats(opcode);
}
