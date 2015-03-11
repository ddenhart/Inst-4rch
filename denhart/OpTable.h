/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    OpTable.cpp
Date:			    03/02/2015
Description:	 This file contains the classes Opcode7inst, Opcode7List, OpRow and 
                OpTableHandle
================================================================================== */

#ifndef __OPTABLE_H_INCLUDED__
#define __OPTABLE_H_INCLUDED__

//Dependencies
//================================================================================== 
class OctConv;
class BitReg;
class Accumulator;

//Defines
//================================================================================== 
//#define DEBUG_OP 


//Class Opcode7inst
//================================================================================== 
class Opcode7inst
{
private:
    unsigned int m_iCode;
    unsigned int m_iTotalCycles;
    unsigned int m_iTotalUsed;
    char* m_sMnemonic;
    BitReg m_rValue;

public:
    Opcode7inst();
    ~Opcode7inst();
    void setRow(unsigned int code, const char* mnem);
    char* getname();
    unsigned int getcode();
    unsigned int getTotalCycles();
    unsigned int getTotalUsed();
    void incCycles();
    void incUsed();
    void printRowStats();
};


//Class Opcode7List
//================================================================================== 
class Opcode7List
{
private:
    Opcode7inst m_op7List[OP7_LIST_LENG];
    unsigned int m_TotalUsed;
    unsigned int m_iTotalCycles;

public:
    Opcode7List();
    ~Opcode7List();
    unsigned int getTotalCycles();
    unsigned int getTotalUsed();
    unsigned int findMicroOp(unsigned int inst);
    unsigned int findIndex(char* name);
    void updateStats(unsigned int inst);
    void nop();
    void print7Stats();
};


//Class OpRow
//================================================================================== 
class OpRow
{
private:
    unsigned int m_iTotalCycles;
    char* m_sMnemonic;
    unsigned int m_iCycles;
    unsigned int m_iTotalUsed;
    BitReg m_rOpcode;

    void fillOpRow();
    void clearOpRow();

public:
    OpRow();
    OpRow(const char* mnem, unsigned int cycles, unsigned int index);
    ~OpRow();
    char* getName();
    unsigned int getOpcode();
    unsigned int getCycles();
    unsigned int getTotal();
    unsigned int getUsage();
    unsigned int incOps();
    void incOne();
    void setRow(const char* mnem, unsigned int cycles, unsigned int index);
    void printRowStats();
    void printRow();
};


//Class OpTableHandle
//================================================================================== 
class OpTableHandle
{
private:
    OpRow m_opTable[OP_TABLE_LENG];
    Opcode7List m_op7;
    Accumulator* m_alu;
    unsigned int m_totalCycles;
    unsigned int m_totalInstructions;

    int searchByName(char* name);

public:
    OpTableHandle();
    OpTableHandle(Accumulator* mem);
    ~OpTableHandle();
    char* getMnemonic(unsigned int index);
    unsigned int getIndex(char* name);
    unsigned int getTotalOpsCycles(unsigned int index);
    unsigned int getTotalCycles();
    unsigned int getTotalUsed(char* name);
    void incrementOpsCycle(unsigned int index);
    void incrementOneCycle(unsigned int index);
    void incrementInstructions();
    void printUsageAndCycles();
    void printTable();
    void updateMicros(unsigned int opcode);
};


#endif //__OPTABLE_H_INCLUDED__

