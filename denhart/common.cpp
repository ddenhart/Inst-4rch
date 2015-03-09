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

    const char* temp1 = "No error";
    const char* temp2 = "Conversion failed";
    const char* temp3 = "Null Pointer";
    const char* temp4 = "Out of Range";
    const char* temp5 = "Unexpected Value";
    const char* temp6 = "Can't access file";
    const char* temp7 = "Deconstructor error";

    errorTable[0] = new char[strlen(temp1)+1];
    strcpy(errorTable[0], temp1);
    errorTable[1] = new char[strlen(temp2)+1];
    strcpy(errorTable[1], temp2);
    errorTable[2] = new char[strlen(temp3)+1];
    strcpy(errorTable[2], temp3);
    errorTable[3] = new char[strlen(temp4)+1];
    strcpy(errorTable[3], temp4);
    errorTable[4] = new char[strlen(temp5)+1];
    strcpy(errorTable[4], temp5);
    errorTable[5] = new char[strlen(temp6)+1];
    strcpy(errorTable[5], temp6);
    errorTable[6] = new char[strlen(temp7)+1];
    strcpy(errorTable[6], temp7);

    const char* file1 = "main";
    const char* file2 = "Common";
    const char* file3 = "OctConv";
    const char* file4 = "BitReg";
    const char* file5 = "Memory";
    const char* file6 = "Accumulator";
    const char* file7 = "ControlUnit";
    const char* file8 = "TableHandle";

    fileTable[0] = new char[strlen(file1)+1];
    strcpy(fileTable[0], file1);
    fileTable[1] = new char[strlen(file2)+1];
    strcpy(fileTable[1], file2);
    fileTable[2] = new char[strlen(file3)+1];
    strcpy(fileTable[2], file3);
    fileTable[3] = new char[strlen(file4)+1];
    strcpy(fileTable[3], file4);
    fileTable[4] = new char[strlen(file5)+1];
    strcpy(fileTable[4], file5);
    fileTable[5] = new char[strlen(file6)+1];
    strcpy(fileTable[5], file6);
    fileTable[6] = new char[strlen(file7)+1];
    strcpy(fileTable[6], file7);
    fileTable[7] = new char[strlen(file8)+1];
    strcpy(fileTable[7], file8);
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

    if( (iErr > 0) && (iErr < ERROR_MAX) && (fileCode > 0) && (fileCode < FILE_MAX) && (errorsOn))
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

