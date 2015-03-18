/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke

==================================================================================
File:			    controlunit.cpp
Date:			    03/02/2015
Description:	 This file contains the classes InstFormat, EffectiveAddress,
ControlUnit and Statistics
================================================================================== */

#ifndef __CONTROLUNIT_H_INCLUDED__
#define __CONTROLUNIT_H_INCLUDED__

//Dependencies
//================================================================================== 
class optable;
class memarray;
class alu;

//Defines
//================================================================================== 
//#define DEBUG_CONTROL 


//Class InstFormat
//================================================================================== 
class InstFormat
{
private:
    unsigned short rIR;  //3 bit instruction register, holds opcode of current instruction
    unsigned short m_rInstruction;
    bool m_bMRI; //memarray reference instruction format, opcodes 0-5
    bool m_bOperate; //instructions that operate on alu and link, opcode 7
    bool m_bTestIO; //input/output instructions, opcode 6
    optable* m_opTable;
    unsigned short m_iMicroCode;

    void setInstType();
    void setIR(unsigned short inst);

public:
    InstFormat();
    ~InstFormat();
    void reset(); //resets the format boolean types
    void loadInstruction(unsigned short inst);
    char* getInstType(); //returns the type of format in a string 
    bool isInstMRI();
    bool isInstOperate();
    bool isInstTestIO();
    //unsigned short getIOdeviceNum(); //TestIO format, 6 bit device number
    //unsigned short getIOfcn(); //TestIO format, 3 bit fcn
    unsigned short getOpcode();
    unsigned short getInstruction();
    char* getInstructionName(unsigned short opcode);
    void incrementCycles(unsigned short  ops);
    void incrementCyclesDefer(unsigned short  ops);
    void incrementInstUsage(unsigned short opcode);
    void printStats();
};


//Class EffectiveAddress
//================================================================================== 
class EffectiveAddress
{
private:
    unsigned short m_rpc; //copy of the current pc
    unsigned short m_rOffset; //7 bit offset
    unsigned short m_rZeroPage; //5 bit, the first memarray page
    unsigned short m_rCurrPage;  //5 bit, the current page
    unsigned short m_rResultAddress; //12 bit resulting address
    bool m_indirect;
    memarray *m_pmem;
    void setPCOffset(unsigned short rpc);
    unsigned short effAdzeroPage();  //if the memarray page bit is 0, calculate 00000 + offset, range is 0000o-0177o
    unsigned short effAdcurrentPage(); //if the memarray page bit is 1, calculate instruction page + offset, range instruction page
    unsigned short effAdindirectAddressZero(); //if the indirect bit is 1, calculate  c(address), range is any address but costs 2 mem reads
    unsigned short effAdindirectAddressCurr(); //if the indirect bit is 1, calculate  c(address), range is any address but costs 2 mem reads
    void loadOffset(unsigned short reg); //extracts the offset from an MRI instruction
    unsigned short readAddress(unsigned short address); //reads the new address from memory

public:
    EffectiveAddress();
    ~EffectiveAddress();  
    //returns 12 bit address of a memarray location from an instruction
    unsigned short geteffAddress(unsigned short reg, unsigned short rpc);
    void setmem(memarray* mem);
    bool isIndirect();
};


//Class ControlUnit
//================================================================================== 
class ControlUnit
{
private:
 
    unsigned short rPC; //12 bit program counter, holds address next instruction
    //unsigned short rSR;  //12 bit console switch register, used to load memarray from console
    bool silent;
    bool running;
    FILE* readfile;
    unsigned short m_StartAddress;
    char m_symbolTable[SYMBOLS_MAX];
    InstFormat m_format;
    EffectiveAddress m_eAddy;
    memarray* m_mem;
    alu* m_alu;

    unsigned short getPC();
    void setPC(unsigned short addy);
    unsigned short hexAddressHandle(char* addy);
    void modeHex(char* filename);
    void modeBin(char* filename);
    void modeOct(char* filename);
    void instructionDefer(); //used to add extra cycles for indirection
    unsigned short readData(unsigned short address);
    void initSymbols();
    short convToNumber(char sIn);

public:
    ControlUnit();
    ~ControlUnit();
    void instructionFetch();
    void instructionDecode();
    void executeMicro(unsigned short inst);
    void executeMRI(unsigned short addy, unsigned short data);
    void loadFile(char* in, short mode);
    void printMemoryHistory();
    void incrementPC();
};


#endif //__CONTROLUNIT_H_INCLUDED__

