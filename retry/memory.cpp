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

#include <fstream>
#include "Common.h"
#include "memory.h"

//External objects
//================================================================================== 
extern ErrorTable Error;


//================================================================================== 
//Description: constructor
//================================================================================== 
memarray::memarray()
{
    const char* defaultfile = "memarrayTrace.dmp";
    int strsize = strlen(defaultfile) + 1;
    traceFileName = new char[strsize];
    strcpy_s(traceFileName, strsize, defaultfile);
    fopen_s(&tracefile, traceFileName, "a");
}


//================================================================================== 
//Description: copy constructor
//================================================================================== 
memarray::memarray(const memarray &source)
{
    int strsize = strlen(source.traceFileName) + 1;
    traceFileName = new char[strsize];
    strcpy_s(traceFileName, strsize, source.traceFileName);
    fopen_s(&tracefile, traceFileName, "a");

    for(short i = 0; i < MSIZE; ++i)
    {
        mem[i].value = source.mem[i].value;
        mem[i].access = source.mem[i].access;
    }
}


//================================================================================== 
//Description: deconstructor
//================================================================================== 
memarray::~memarray()
{
    if(tracefile)
    {
        fclose(tracefile);
        tracefile = NULL;
    }


    if(traceFileName)
    {
        delete[] traceFileName;
        traceFileName = NULL;
    }
}


//================================================================================== 
//Description: writes the value in MB to the address in MA in the memarray array
//================================================================================== 
void memarray::writeline()
{
    rMA = checkValidAddy(rMA);
    rMB = checkValidAddy(rMB);
    mem[rMA].value = rMB; //write the memarray line
    mem[rMA].access = true; //change the access flag to true
}


//================================================================================== 
//Description: saves data from MB to the memarray array at the address from MA
//================================================================================== 
void memarray::readline()
{
    rMA = checkValidAddy(rMA);
    rMB = checkValidAddy(rMB);
    rMB = mem[rMA].value; //read the memarray line
}


//================================================================================== 
//Description: writes all memarray lines that have been accessed to an external file
//================================================================================== 
void memarray::writeMemoryAccesses()
{
    char filename[] = "pdp8_Memory_Accesses.txt";

    fopen_s(&memfile, filename, "a");

    if(memfile)
    {
        fprintf(memfile, "%s\n", PRINT_BREAK);
        fprintf(memfile, "PDP-8 Memory\n"); //print a header
        fprintf(memfile, "%s\n", PRINT_BREAK);
        fprintf(memfile, "Address  Data\n");
        fprintf(memfile, "%s\n", PRINT_BREAK);

        for(short i = 0; i < MSIZE; ++i)
        {
            if(mem[i].access) //if the memarray has been used
            {
                fprintf(memfile, "%o      %o\n", i, mem[i].value);  //print the address and value
            }
        }

        fclose(memfile);
    }
    else
    {
        Error.printError(ERROR_FILE, FILE_MEM);
    }
}


//================================================================================== 
//Description: writes an address to the MA and value to the MB and log it
//================================================================================== 
void memarray::store(unsigned short address, unsigned short value)
{
    short type = MWRITE;

    address = checkValidAddy(address); //check for valid address
    value = checkValidAddy(value);
    rMA = address; //set the address in the MA
    rMB = value; //set the data in the MB
    writeline(); //initiate the write
    logtrace(address, type); //log the memarray 
}


//================================================================================== 
//Description:loads an address to the MA, initiates a read and logs it
//================================================================================== 
void memarray::load(unsigned short address)
{
    short type = MREAD;

    address = checkValidAddy(address);  //check for valid address
    rMA = address; //write the address to the MA
    readline(); //initiate the memarray read
    logtrace(address, type); //log the memarray access
}


//================================================================================== 
//Description: log a fetched address and store it in memarray
//================================================================================== 
void memarray::fetch(unsigned short address)
{
    short type = MFETCH;

    address = checkValidAddy(address); //check for valid address
    logtrace(address, type); //log the memarray access
}


//================================================================================== 
//Description: retrieves the access flag
//================================================================================== 
bool memarray::getAccess(unsigned short addy)
{
    bool bRes = 0;
    addy = checkValidAddy(addy);  //check for valid address
    bRes = mem[addy].access; //copy the flag

    return bRes;
}


//================================================================================== 
//Description: checks if an address is within bounds of the memarray array
//================================================================================== 
unsigned short memarray::checkValidAddy(unsigned short addy)
{
    unsigned short temp = 0;

    if(MAX_OCT_ADDRESS > addy)
    {
        temp = addy;
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_MEM);
        temp = MAX_OCT_ADDRESS - addy;
        //temp = REG_12BIT_MASK & addy;
#ifdef DEBUG_MEM
        fprintf(stdout, "DEBUG: checking memory address from %o to %o\n", addy, temp);
#endif
    }

    return temp;
}


//================================================================================== 
//Inputs: unsigned short pointer holding the relevant address
//================================================================================== 
void memarray::setAccess(unsigned short addy)
{
    addy = checkValidAddy(addy);
    mem[addy].access = true;
}


//================================================================================== 
//Description: prints reads, writes or fetches to an external file
//================================================================================== 
void memarray::logtrace(unsigned short address, short type)
{
    address = checkValidAddy(address);
    if(tracefile)
    {
        //write the type and address to the file
        fprintf(tracefile, "%d   %o\n", type, address); 
    }
    else
    {
        Error.printError(ERROR_FILE, FILE_MEM);
    }
}


//================================================================================== 
//Description: reads the memarray buffer register 
//================================================================================== 
unsigned short memarray::readMB()
{
    return rMB;
}


//================================================================================== 
//Description: gets the string name for the memarray operation type
//================================================================================== 
char* memarray::getMemoryOp(short type)
{
    const char* temp1 = "READ";
    const char* temp2 = "WRITE";
    const char* temp3 = "FETCH";
    const char* temp4 = "NO VALUE";
    char* temp = NULL;
    int strsize = 0;

    if(MREAD == type)
    {
        strsize = strlen(temp1)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, temp1);
    }
    else if(MWRITE == type)
    {
        strsize = strlen(temp2)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, temp2);
    }
    else if(MFETCH == type)
    {
        strsize = strlen(temp3)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, temp3);
    }
    else
    {
        strsize = strlen(temp4)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, temp4);
    }

    return temp;
}

