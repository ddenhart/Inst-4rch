/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    Common.cpp
Date:			    03/02/2015
Description:	 This file contains the classes ErrorTable and RegTable
================================================================================== */

#include <fstream>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "OctConv.h"
#include "BitReg.h"
#include "Common.h"

//External objects
//================================================================================== 
extern ErrorTable Error; 
extern RegTable RegisterFile;


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
ErrorTable::ErrorTable()
{
    errorsOn = true;

    errorTable[0] = "No error";
    errorTable[1] = "Conversion failed";
    errorTable[2] = "Null Pointer";
    errorTable[3] = "Out of Range";
    errorTable[4] = "Unexpected Value";
    errorTable[5] = "Can't access file";
    errorTable[6] = "Max error";

    fileTable[0] = "main";
    fileTable[1] = "Common";
    fileTable[2] = "OctConv";
    fileTable[3] = "BitReg";
    fileTable[4] = "Memory";
    fileTable[5] = "Accumulator";
    fileTable[6] = "ControlUnit";
    fileTable[7] = "OpRow";
    fileTable[8] = "TableHandle";
    fileTable[9] = "RegTable";
    fileTable[10] = "file max";
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
ErrorTable::~ErrorTable()
{

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ErrorTable::printError(int error, int fileCode)
{
    int iErr = abs(error);

    if( (iErr > 0) && (iErr < ERROR_MAX) && (fileCode > 0) && (fileCode < FILE_MAX) )
    {
        fprintf(stderr, "%s\n", PRINT_BREAK);
        fprintf(stderr, "***Error found in %s: %s***\n", fileTable[fileCode], errorTable[iErr]);
        fprintf(stderr, "%s\n", PRINT_BREAK);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ErrorTable::debugOn(bool bSwitch)
{
    errorsOn = bSwitch;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
RegTable::RegTable()
{
    rMA = new BitReg(REG_12BIT);
    rMA->initArray();
    rMB = new BitReg(REG_12BIT);
    rMB->initArray();
    rAC = new BitReg(REG_12BIT);
    rAC->initArray();
    rLB = new BitReg(REG_1BIT);
    rLB->initArray();
    rPC = new BitReg(REG_12BIT);
    rPC->initArray();
    rIR = new BitReg(REG_3BIT);
    rIR->initArray();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
RegTable::~RegTable()
{
    if(rMA)
    {
        delete rMA;
        rMA = NULL;
    }
    if(rMB)
    {
        delete rMB;
        rMB = NULL;
    }
    if(rAC)
    {
        delete rAC;
        rAC = NULL;
    }
    if(rLB)
    {
        delete rLB;
        rLB = NULL;
    }
    if(rPC)
    {
        delete rPC;
        rPC = NULL;
    }
    if(rIR)
    {
        delete rIR;
        rIR = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void RegTable::incrementPC()
{
    BitReg address(RegisterFile.rPC->getBool());
    int current = address.getNumber();
    current += INCREMENT_WORD;
    rPC->setReg(current);
}

