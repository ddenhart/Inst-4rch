/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    Memory.cpp
Date:			    03/02/2015
Description:	 This file contains the classes memReg and Memory
================================================================================== */

#ifndef __MEMORY_H_INCLUDED__
#define __MEMORY_H_INCLUDED__

//Dependencies
//================================================================================== 
class BitReg;
class fstream;

//Defines
//================================================================================== 
//#define DEBUG_MEMORY 
#define MREAD 0
#define MWRITE 1
#define MFETCH 2
#define NONVAL -1

//Consts
//================================================================================== 
const int MSIZE = 4096;


//Struct Word
//================================================================================== 
struct memReg
{
    BitReg* value;
    bool access;
    memReg();
    ~memReg();
};


//Class Memory
//================================================================================== 
class Memory
{
private:
    memReg memory[MSIZE];
    char* traceFileName;
    FILE* tracefile;
    FILE* memfile;

    void writeline();
    void readline();
    void logtrace(BitReg* address, int type);

public:
    Memory();
    ~Memory();
    Memory(const Memory &source);
    void store(BitReg* addy, BitReg* value);
    void load(BitReg* addy);
    void fetch(BitReg* addy);
    bool getAccess(BitReg* addy);
    bool checkValidAddy(BitReg* addy);
    void setAccess(BitReg* addy);
    void writeMemoryAccesses(char* filename);
    BitReg* readMB();
    bool pcMemoryValid();
    char* getMemoryOp(int type);
};


#endif //__MEMORY_H_INCLUDED__

