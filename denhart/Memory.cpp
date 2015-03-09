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

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Constructor for struct Word
Word::Word()
{
    value = new BitReg(REG_12BIT);
    value->initArray();
    access = false;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Word::~Word() 
{
    if(value)
    {
        delete value; 
        value = NULL;
    }
    access = false;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Constructor for class Memory
// INPUT: number of memory locations to create
Memory::Memory()
{
    logfile_name = new char[strlen(default_memfile) + 1];
    strcpy(logfile_name, default_memfile);
    logfile.open(logfile_name, std::ios::app);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Destructor for class Memory
Memory::~Memory()
{
    logfile.close();

    if(logfile_name)
    {
        delete[] logfile_name;
        logfile_name = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Put value into memory
void Memory::mem_put()
{
    bool* baddy = NULL;
    bool* bdata = NULL;
    baddy = RegisterFile.rMA->getBool();
    bdata = RegisterFile.rMB->getBool();
    if(baddy)
    {
        if(bdata)
        {
            BitReg address(baddy, REG_12BIT);
            BitReg data(bdata, REG_12BIT);
            int iAddy = address.getNumber();
            // Insert value at address
            memory[iAddy].value->setReg(&data);
            setAccess(&address);
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_MEMORY);
        }
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
    if(baddy)
    {
        delete[] baddy;
        baddy = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Get value from memory
void Memory::mem_get()
{
    bool* bdata = NULL;
    bool* baddress = NULL;

    baddress = RegisterFile.rMA->getBool();
    BitReg address(baddress);
    int iAddy = address.getNumber();
    bdata = memory[iAddy].value->getBool();
    BitReg data(bdata);
    RegisterFile.rMB->setReg(&data);

    if(bdata)
    {
        delete[] bdata;
        bdata = NULL;
    }

    if(baddress)
    {
        delete[] baddress;
        baddress = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Display all memory locations that have been previously accessed
// INPUT: ostream to display on
// OUTPUT: number of memory locations displayed
int Memory::dump_memory(char* filename)
{
    int count = 0;  // To count lines displayed
    char* value = NULL;

    memfile.open(filename, std::ios::app);

    // Setup stream format
    memfile << std::oct << std::setfill('0') << std::right;

    for (int i = 0; i < MSIZE; ++i)
    {
        if(memory[i].access)
        {
            value = memory[i].value->getString(REG_4BIT);
            memfile << std::setw(5) << i << ": ";
            memfile << std::setw(5) << value << std::endl;
            ++count;
        }

        if(value)
        {
            delete[] value;
            value = NULL;
        }
    }
    
    memfile.close();
    return count;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Store value in memory and log
// INPUT: address and value
// OUTPUT: logged as a write
void Memory::store(BitReg* address, BitReg* value)
{
    log_type type(MWRITE);

    if(address && value)
    {
        // Check that address is valid
        if(!checkValidAddy(address))
        {
            throw std::out_of_range("Array out of bounds");
            return;
        }

        RegisterFile.rMA->setReg(address);
        RegisterFile.rMB->setReg(value);
        mem_put();
        setAccess(address);
        log(address, type);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Load value from memory and log
// INPUT: address
// read the rMB to get the value
void Memory::load(BitReg* address)
{
    if(address)
    {
        log_type type(MREAD);
   
        if (!checkValidAddy(address))
        {
            throw std::out_of_range ("Array out of bounds");
            return;
        }

        RegisterFile.rMA->setReg(address);
        mem_get();
        log(address, type);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }
    
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// log an address that was fetched
// INPUT: address
// read the rMB to get the value
void Memory::fetch(BitReg* address)
{
    if(address)
    {
        log_type type(MFETCH);

        if(!checkValidAddy(address))
        {
            throw std::out_of_range("Array out of bounds");
            return;
        }

        log(address, type);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_MEMORY);
    }

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool Memory::getAccess(BitReg* addy)
{
    bool bRes = 0;

    if(addy)
    {
        int iAddy = addy->getNumber();

        if(checkValidAddy(addy))
        {
            bRes = memory[iAddy].access;
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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool Memory::checkValidAddy(BitReg* addy)
{
    bool bRes = false;

    int iAddy = addy->getNumber();

    if(addy)
    { 
        if((MSIZE > iAddy) && (iAddy >= 0))
        {
            bRes = true;
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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Memory::setAccess(BitReg* addy)
{
    int iAddy = 0;
    
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
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Log memory access
// INPUT: address and type
// OUTPUT: none
void Memory::log(BitReg* address, log_type type)
{
    char* name = NULL;
        
    name = address->getString(REG_4BIT);

    if(address)
    {
        if(logfile.is_open())
        {
            logfile << type.getName() << " " << name << std::endl;
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

    if(name)
    {
        delete[] name;
        name = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Log memory access
// INPUT: address and type
// OUTPUT: none
BitReg* Memory::readMB()
{
    return RegisterFile.rMB->getReg();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
log_type::log_type()
{
    type = -1;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
log_type::log_type(int itype)
{
    type = itype;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
log_type::~log_type()
{
   
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
char* log_type::getName()
{
    if(MREAD == type)
    {
        return "READ";
    }
    else if(MWRITE == type)
    {
        return "WRITE";
    }
    else if(MFETCH == type)
    {
        return "FETCH";
    }
    else
    {
        return "NO VALUE";
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void log_type::setType(int value)
{
    if(MREAD == value)
    {
        type = MREAD;
    }
    else if(MWRITE == value)
    {
        type = MWRITE;
    }
    else if(MFETCH == value)
    {
        type = MFETCH;
    }
    else
    {
        type = NONVAL;
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_MEMORY);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int log_type::getType()
{
    return type;
}

