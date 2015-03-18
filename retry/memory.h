/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke

==================================================================================
File:			    memarray.cpp
Date:			    03/02/2015
Description:	 This file contains the classes memReg and memarray
================================================================================== */

#ifndef __memarray_H_INCLUDED__
#define __memarray_H_INCLUDED__

//Dependencies
//================================================================================== 
class fstream;

//Defines
//================================================================================== 
//#define DEBUG_MEM
#define MREAD 0
#define MWRITE 1
#define MFETCH 2
#define NONVAL -1

//Consts
//================================================================================== 
const short MSIZE = 4096;
const char tracename[] = "memdump.dmp\0";



//Class memarray
//================================================================================== 
class memarray
{
private:
    struct memReg
    {
        unsigned short value;
        bool access;
    };

    memReg mem[MSIZE];
    //Reading and writing memarray
    //to read memarray, a 12 bit address is loaded into the MA and
    //a read signal is asserted. The contents appear in the MB
    unsigned short rMA; //12 bit central processor memarray address register
    unsigned short rMB; //12 bit memarray Buffer register
    char* traceFileName;
    FILE* tracefile;
    FILE* memfile;

    memarray(const memarray &source);
    void writeline();
    void readline();
    void logtrace(unsigned short address, short type);
    char* getMemoryOp(short type);

public:
    memarray();
    ~memarray();
    void store(unsigned short addy, unsigned short value);
    void load(unsigned short addy);
    void fetch(unsigned short addy);
    bool getAccess(unsigned short addy);
    void checkValidAddy(unsigned short addy);
    void setAccess(unsigned short addy);
    void writeMemoryAccesses();
    unsigned short readMB();
    bool pcMemoryValid(unsigned short rpc);
};


#endif //__memarray_H_INCLUDED__

