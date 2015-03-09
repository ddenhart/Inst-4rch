/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    Memory.cpp
Date:			    03/02/2015
Description:	 This file contains the classes WORD, Memory and log-type
================================================================================== */

#ifndef __MEMORY_H_INCLUDED__
#define __MEMORY_H_INCLUDED__

//Dependencies
//================================================================================== 
class BitReg;
class fstream;

//Defines
//================================================================================== 
#define DEBUG_MEMORY true
#define MREAD 0
#define MWRITE 1
#define MFETCH 2
#define NONVAL -1

//Consts
//================================================================================== 
const int MSIZE = 4096;


//Class log_type
//================================================================================== 
class log_type
{
private:
    int type;

public:
    log_type();
    log_type(int type);
    ~log_type();
    char* getName();
    void setType(int value);
    int getType();
};


//Struct Word
//================================================================================== 
struct Word
{
    BitReg* value;
    bool access;
    Word();
    ~Word();
};


//Class Memory
//================================================================================== 
class Memory
{
private:
    Word memory[MSIZE];
    char* logfile_name;
    std::ofstream logfile;
    std::ofstream memfile;

    void mem_put();
    void mem_get();
    void log(BitReg* address, log_type type);

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
    int dump_memory(char* filename);
    BitReg* readMB();
};


#endif //__MEMORY_H_INCLUDED__

