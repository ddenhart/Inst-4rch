/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    ControlUnit.cpp
Date:			    03/02/2015
Description:	 This file contains the classes InstFormat, EffectiveAddress,
ControlUnit and Statistics
================================================================================== */

#ifndef __CONTROLUNIT_H_INCLUDED__
#define __CONTROLUNIT_H_INCLUDED__

//Dependencies
//================================================================================== 
class BitReg;
class OpTableHandle;
class Memory;
class Accumulator;

//Defines
//================================================================================== 
#define DEBUG_CONTROL true

//Class InstFormat
//================================================================================== 
class InstFormat
{
private:
    BitReg* m_rInstruction;
    BitReg* m_rCurrAddy;
    BitReg* m_rOpcode; //3 bit opcode
    BitReg* m_rOffset; //7 bit offset
    BitReg* m_rOffext; //9 bit offset
    bool m_bMRI; //memory reference instruction format, opcodes 0-5
    bool m_bOperate; //instructions that operate on accumulator and link, opcode 7
    bool m_bTestIO; //input/output instructions, opcode 6
    bool m_bIndirect;
    bool m_bZeroPage;
    OpTableHandle* m_opTable;
    int m_iMicroCode;
    
    void setOpCode();
    void setOffset();
    void setInstType();

public:
    InstFormat();
    ~InstFormat();
    void reset(); //resets the format boolean types
    void loadInstruction(BitReg* inst);
    char* getInstType(); //returns the type of format in a string 
    bool getMRIindirect(); //MRI format, 1 bit Indirect bit, 0 = none, 1 = indirection
    bool getMRIMempage(); //MRI format, 1 bit Memory page bit, 0 = zero page, 1 = current page
    BitReg* getMRIoffset(); //MRI format, 7 bit offset
    bool isInstMRI();
    bool isInstOperate();
    bool isInstTestIO();
    BitReg* getOPextended(); //Operate format, 9 bit extended opcode
    //BitReg getIOdeviceNum(); //TestIO format, 6 bit device number
    //BitReg getIOfcn(); //TestIO format, 3 bit fcn
    int getMicroCode();
    int getOpcode();
    BitReg* getInstruction();
    BitReg* getAddress();
    void setAddress(BitReg* addy);
};


//Class EffectiveAddress
//================================================================================== 
class EffectiveAddress
{
private:
    Memory* m_pMemory;
    BitReg* m_rOffset; //7 bit offset
    BitReg* m_rZeroPage; //5 bit, the first memory page
    BitReg* m_rCurrPage;  //5 bit, the current page
    BitReg* m_rResultAddress; //12 bit resulting address
    bool m_bIndirect;
    bool m_bMemory;

    void setPCOffset();
    BitReg* effAdzeroPage();  //if the memory page bit is 0, calculate 00000 + offset, range is 0000o-0177o
    BitReg* effAdcurrentPage(); //if the memory page bit is 1, calculate instruction page + offset, range instruction page
    BitReg* effAdindirectAddressZero(); //if the indirect bit is 1, calculate  c(address), range is any address but costs 2 mem reads
    BitReg* effAdindirectAddressCurr(); //if the indirect bit is 1, calculate  c(address), range is any address but costs 2 mem reads
    BitReg* effAdautoIndexing(); //calculate c(0010o - 0017o) + 1 effective address
    void loadOffset(BitReg* reg);
    BitReg* readIndirect(BitReg* reg);

public:
    EffectiveAddress();
    ~EffectiveAddress();
    void setMemory(Memory* memory);
    BitReg* getAddress(BitReg* reg);  //returns 12 bit address of a memory location from an instruction
};


//Class ControlUnit
//================================================================================== 
class ControlUnit
{
private:
    InstFormat m_format;
    EffectiveAddress m_eAddy;
    Memory* m_memory;
    Accumulator* m_alu;

    void instructionDefer(); //used to add extra cycles for indirection
    BitReg* getPC();
    void setPC(BitReg* addy);
    BitReg* getAC();
    void setAC(BitReg* addy);

public:
    ControlUnit();
    ~ControlUnit();
    void instructionFetch(BitReg* reg);
    void instructionDecode();
    void instructionExecute();
    void load_from_file(char* filename);
    void printMemoryHistory(char* filename);
};

//TODO: statistics
//Class Statistics
//================================================================================== 


#endif //__CONTROLUNIT_H_INCLUDED__

