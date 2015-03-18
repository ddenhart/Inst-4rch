/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    OpTable.cpp
Date:			    03/02/2015
Description:	 This file contains the classes Opcode7inst, Opcode7List, OpRow and 
                optable
================================================================================== */

#ifndef __OPTABLE_H_INCLUDED__
#define __OPTABLE_H_INCLUDED__

//Dependencies
//================================================================================== 
class alu;

//Defines
//================================================================================== 
//#define DEBUG_OP 

//Class OpRow
//================================================================================== 
class OpRow
{
private:
    unsigned short m_iTotalCycles;
    char* m_sMnemonic;
    unsigned short m_iCycles;
    unsigned short m_iTotalUsed;
    unsigned short m_rOpcode;

    void fillOpRow();
    void clearOpRow();

public:
    OpRow();
    OpRow(const char* mnem, unsigned short cycles, unsigned short index);
    ~OpRow();
    char* getName();
    unsigned short getOpcode();
    unsigned short getCycles();
    unsigned short getTotalCycles();
    unsigned short getTotalUsage();
    unsigned short incOpsCycle();
    void incOneCycle();
    void setRow(const char* mnem, unsigned short cycles, unsigned short index);
    void printRowStats();
};


//Class optable
//================================================================================== 
class optable
{
private:
    OpRow m_opTable[OP_TABLE_LENG];
    //Opcode7List m_op7;
    alu* m_alu;
    unsigned short m_totalCycles;
    unsigned short m_totalInstructions;

public:
    optable();
    optable(alu* mem);
    ~optable();
    char* getMnemonic(unsigned short index);
    unsigned short getIndex(char* name);
    unsigned short getTotalOpsCycles(unsigned short index);
    unsigned short getTotalCycles();
    unsigned short getTotalUsed(char* name);
    void incrementOpsCycle(unsigned short index);
    void incrementOneCycle(unsigned short index);
    void printUsageAndCycles();
};


#endif //__OPTABLE_H_INCLUDED__

