/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
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

//Defines
//================================================================================== 
#define OP_TABLE_LENG 12
#define OP7_LIST_LENG 11


//Class Opcode7inst
//================================================================================== 
class Opcode7inst
{

private:
    int m_iCode;
    char* m_sMnemonic;
    BitReg m_rValue;

public:
    Opcode7inst();
    Opcode7inst(int code, char* mnem);
    Opcode7inst(Opcode7inst& source);
    ~Opcode7inst();
};


//Class Opcode7List
//================================================================================== 
class Opcode7List
{
private:
    Opcode7inst m_op7List[OP7_LIST_LENG];

public:
    Opcode7List();
    ~Opcode7List();
    void printList();
};


//Class OpRow
//================================================================================== 
class OpRow
{
private:
    int m_iTotalCount;
    char* m_sMnemonic;
    int m_iCycles;
    BitReg m_rOpcode;

    void fillOpRow();
    void clearOpRow();

public:
    OpRow();
    OpRow(char* mnem, int cycles, unsigned int index);
    ~OpRow();
    char* getName();
    int getOpcode();
    int getCycles();
    int getTotal();
    void incTotal();
    void setRow(char* mnem, int cycles, unsigned int index);
    void printRow();
};


//Class OpTableHandle
//================================================================================== 
class OpTableHandle
{
private:
    OpRow m_opTable[OP_TABLE_LENG];
    Opcode7inst m_op7;
    int searchByName(char* name);

public:
    OpTableHandle();
    ~OpTableHandle();
    char* getMnemonic(int index);
    int getIndex(char* name);
    int getTotal(int index);
    void incrementTotal(int index);
    void printTable();
};


#endif //__OPTABLE_H_INCLUDED__

