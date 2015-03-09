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
class Accumulator;

//Defines
//================================================================================== 
#define DEBUT_OP true
#define OP_TABLE_LENG 12
#define OP7_LIST_LENG 11


//Class Opcode7inst
//================================================================================== 
class Opcode7inst
{

private:
    unsigned int m_iCode;
    char* m_sMnemonic;
    BitReg m_rValue;

public:
    Opcode7inst();
    ~Opcode7inst();
    void setRow(unsigned int code, const char* mnem);
    unsigned int getcode();
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
    int findMicroOp(unsigned int inst);
    void nop();
    //void iac();
    //void rar();
    //void ral();
    //void rtl();
    //void rtr();
    //void bsw();
    //void cml();
    //void cma();
    //void cia();
    void cll();
    //void stl();
    void cla();
    //void sta();
    void hlt(); //group 2
    /*void osr();
    void skp();
    void snl();
    void szl();
    void sza();
    void sna();
    void sma();
    void spa();
    void cam(); //group 3
    void mqa();
    void mql();
    void swp();
    void printList();*/
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
    OpRow(const char* mnem, int cycles, unsigned int index);
    ~OpRow();
    char* getName();
    unsigned int getOpcode();
    int getCycles();
    int getTotal();
    void incTotal();
    void setRow(const char* mnem, int cycles, unsigned int index);
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

    int searchByName(char* name);

public:
    OpTableHandle();
    OpTableHandle(Accumulator* mem);
    ~OpTableHandle();
    char* getMnemonic(unsigned int index);
    int getIndex(char* name);
    int getTotal(int index);
    void incrementTotal(int index);
    void printTable();
    void exMicroInst(int opcode);
};


#endif //__OPTABLE_H_INCLUDED__

