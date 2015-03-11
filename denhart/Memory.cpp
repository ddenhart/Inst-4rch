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

#include <fstream>
#include "OctConv.h"
#include "Common.h"
#include "BitReg.h"
#include "Memory.h"

//External objects
//================================================================================== 
extern ErrorTable Error;
extern RegTable RegisterFile;
extern OctConv OctTable;


//================================================================================== 
//Name:memReg
//Description: constructor
//Inputs: none
//Outputs: none
//Return: none
//================================================================================== 
memReg::memReg()
{
    value = new BitReg(REG_12BIT);
    value->initArray();
    access = false;
}


//================================================================================== 
//Name: ~memReg
//Description: deconstructor
//Inputs: none
//Outputs: none
//Return: none
//================================================================================== 
memReg::~memReg()
{
    if(value)
    {
        delete value; 
        value = NULL;
    }
    access = false;
}


//================================================================================== 
//Name: Memory
//Description: constructor
//Inputs: none
//Outputs: none
//Return: none
//================================================================================== 
Memory::Memory()
{
    const char* defaultfile = "memoryTrace.dmp";
    traceFileName = new char[strlen(defaultfile) + 1];
    strcpy(traceFileName, defaultfile);
    tracefile = fopen(traceFileName, "a");
}


//================================================================================== 
//Name: Memory
//Description: copy constructor
//Inputs: constant memory class
//Outputs: none
//Return: none
//================================================================================== 
Memory::Memory(const Memory &source)
{
    traceFileName = new char[strlen(source.traceFileName) + 1];
    strcpy(traceFileName, source.traceFileName);
    tracefile = fopen(traceFileName, "a");

    for(int i = 0; i < MSIZE; ++i)
    {
        memory[i].value->setReg(source.memory[i].value->getReg());
        memory[i].access = source.memory[i].access;
    }
}


//================================================================================== 
//Name: Memory
//Description: deconstructor
//Inputs: none
//Outputs: none
//Return: none
//================================================================================== 
Memory::~Memory()
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
//Name: writeline
//Description: writes the value in MB to the address in MA in the memory array
//Inputs: none
//Outputs: none
//Return: none
//================================================================================== 
void Memory::writeline()
{
    unsigned int iAddy = 0;
    bool* bdata = NULL;
    iAddy = RegisterFile.rMA->getNumber();
    bdata = RegisterFile.rMB->getBool();
   
    if(bdata)
    {
        BitReg address(iAddy, REG_12BIT);
        BitReg data(bdata, REG_12BIT);
        memory[iAddy].value->setReg(&data); //write the memory line
        setAccess(&address); //change the access flag to true
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

    if(bdata)
    {
        delete[] bdata;
        bdata = NULL;
    }
}


//================================================================================== 
//Name: readline
//Description: saves data from MB to the memory array at the address from MA
//Inputs: none
//Outputs: none
//Return: none
//================================================================================== 
void Memory::readline()
{
    bool* bdata = NULL;
    unsigned int iAddy = 0;

    iAddy = RegisterFile.rMA->getNumber(); //get the register address from MA
    bdata = memory[iAddy].value->getBool(); //read the memory line
    BitReg data(bdata);
    RegisterFile.rMB->setReg(&data); //set the MB with the valid data

    if(bdata)
    {
        delete[] bdata;
        bdata = NULL;
    }
}


//================================================================================== 
//Name: writeMemoryAccesses
//Description: writes all memory lines that have been accessed to an external file
//Inputs: the file name
//Outputs: none
//Return: none
//================================================================================== 
void Memory::writeMemoryAccesses(char* filename)
{
    char* value = NULL;

    memfile = fopen(filename, "w");

    if(memfile)
    {
        fprintf(memfile, "%s\n", PRINT_BREAK);
        fprintf(memfile, "PDP-8 Memory\n"); //print a header
        fprintf(memfile, "%s\n", PRINT_BREAK);
        fprintf(memfile, "Address  Data\n"); 
        fprintf(memfile, "%s\n", PRINT_BREAK);

        for(int i = 0; i < MSIZE; ++i)
        {
            if(memory[i].access) //if the memory has been used
            {
                value = memory[i].value->getString(REG_4BIT); //get the value
                fprintf(memfile, "%o      %s\n", i, value);  //print the address and value
                
                if(value) //delete the copy
                {
                    delete[] value;
                    value = NULL;
                }
            }
        }

        fclose(memfile);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }
}


//================================================================================== 
//Name: store
//Description: writes an address to the MA and value to the MB and log it
//Inputs: two BitReg pointers holding the relevant address and data
//Outputs: none
//Return: none
//================================================================================== 
void Memory::store(BitReg* address, BitReg* value)
{
    int type = MWRITE;

    if(address && value)
    {
        checkValidAddy(address); //check for valid address
        RegisterFile.rMA->setReg(address); //set the address in the MA
        RegisterFile.rMB->setReg(value); //set the data in the MB
        writeline(); //initiate the write
        setAccess(address); //set the access flag
        logtrace(address, type); //log the memory access
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }
}


//================================================================================== 
//Name: load
//Description:loads an address to the MA, initiates a read and logs it
//Inputs: BitReg pointer holding the relevant address
//Outputs: none
//Return: none
//================================================================================== 
void Memory::load(BitReg* address)
{
    if(address)
    {
        int type = MREAD;
   
        checkValidAddy(address);  //check for valid address
        RegisterFile.rMA->setReg(address); //write the address to the MA
        readline(); //initiate the memory read
        logtrace(address, type); //log the memory access
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }
    
}


//================================================================================== 
//Name: fetch
//Description: log a fetched address and store it in memory
//Inputs: a BitReg pointer holding the relevant address
//Outputs: none
//Return: none
//================================================================================== 
void Memory::fetch(BitReg* address)
{
    if(address)
    {
        int type = MFETCH;

        checkValidAddy(address); //check for valid address
        logtrace(address, type); //log the memory access
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

}


//================================================================================== 
//Name: getAccess
//Description: retrieves the access flag
//Inputs: BitReg pointer holding the relevant address
//Outputs: none
//Return: true if the memory address has been accessed before, false if unused
//================================================================================== 
bool Memory::getAccess(BitReg* addy)
{
    bool bRes = 0;

    if(addy)
    {
        unsigned int iAddy = addy->getNumber(); //get the address in decimal 

        if(checkValidAddy(addy))  //check for valid address
        {
            bRes = memory[iAddy].access; //copy the flag
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_MEMORY);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

    return bRes;
}


//================================================================================== 
//Name: checkValidAddy
//Description: checks if an address is within bounds of the memory array
//Inputs: BitReg pointer holding the relevant address
//Outputs:
//Return: true if the address is valid, false if the address is out of range
//================================================================================== 
bool Memory::checkValidAddy(BitReg* addy)
{
    bool bRes = false;

    unsigned int iAddy = addy->getNumber();

    if(addy)
    { 
        if(MSIZE > iAddy)
        {
            bRes = true;
        }
        else
        {
            fprintf(stderr, "%s\n", PRINT_BREAK);
            throw fprintf(stderr, "Error: Array out of bounds..");
            fprintf(stderr, "%s\n", PRINT_BREAK);
            return 0;
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

    return bRes;
}


//================================================================================== 
//Name:
//Description:
//Inputs: BitReg pointer holding the relevant address
//Outputs:
//Return:
//================================================================================== 
void Memory::setAccess(BitReg* addy)
{
    unsigned int iAddy = 0;
    
    if(addy)
    {
        iAddy = addy->getNumber();

        if(checkValidAddy(addy))
        {
            memory[iAddy].access = true;
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_MEMORY);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }
}


//================================================================================== 
//Name: log
//Description: prints reads, writes or fetches to an external file
//Inputs: BitReg pointer holding the relevant address, type of memory op, use 
//          macros: MREAD, MWRITE, MFETCH
//Outputs: none
//Return: none
//================================================================================== 
void Memory::logtrace(BitReg* address, int type)
{
    char* addy = NULL;
        

    if(address)
    {
        addy = address->getString(REG_4BIT); //get a cstring of the address
        if(tracefile)
        {
            fprintf(tracefile,"%d   %s\n", type, addy); //write the type and address to the file
        }
        else
        {
            Error.printError(ERROR_FILE, FILE_MEMORY);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

    if(addy)
    {
        delete[] addy;
        addy = NULL;
    }
}


//================================================================================== 
//Name: readMB
//Description: reads the Memory buffer register 
//Inputs: none
//Outputs: none
//Return: a BitReg pointer holding the register's value
//================================================================================== 
BitReg* Memory::readMB()
{
    return RegisterFile.rMB->getReg();
}


//==================================================================================
//Name: pcMemoryValid
//Description: checks if the pc has gone past the last memory address
//Inputs: none
//Outputs: none
//Return: true if pc is inside the memory space, false if outside the array
//==================================================================================
bool Memory::pcMemoryValid()
{
    bool* bpc = NULL;
    bool blimitvalid = false;
    BitReg rpc;

    bpc = RegisterFile.rPC->getBool();
    
    if(bpc)
    {
        rpc.setReg(bpc);
        blimitvalid = checkValidAddy(&rpc);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

    return blimitvalid;
}


//================================================================================== 
//Name: getMemoryOp
//Description: gets the string name for the memory operation type
//Inputs: type of memory op, use macros: MREAD, MWRITE, MFETCH
//Outputs: none
//Return: cstring of the associated memory op type
//================================================================================== 
char* Memory::getMemoryOp(int type)
{
    const char* temp1 = "READ";
    const char* temp2 = "WRITE";
    const char* temp3 = "FETCH";
    const char* temp4 = "NO VALUE";
    char* temp = NULL;

    if(MREAD == type)
    {
        temp = new char[strlen(temp1)+1];
        strcpy(temp, temp1);
    }
    else if(MWRITE == type)
    {
        temp = new char[strlen(temp2)+1];
        strcpy(temp, temp2);
    }
    else if(MFETCH == type)
    {
        temp = new char[strlen(temp3)+1];
        strcpy(temp, temp3);
    }
    else
    {
        temp = new char[strlen(temp4)+1];
        strcpy(temp, temp4);
    }

    return temp;
}

