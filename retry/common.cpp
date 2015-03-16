/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke

==================================================================================
File:			    Common.cpp
Date:			    03/02/2015
Description:	 This file contains the classes ErrorTable and RegTable
================================================================================== */

#include <fstream>
#include <cstdio>
#include "Common.h"

//External objects
//================================================================================== 

//================================================================================== 
//Description: constructor
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
    int strsize = 0;

    strsize = strlen(temp1)+1;
    errorTable[0] = new char[strsize];
    strcpy(errorTable[0], temp1);
    strsize = strlen(temp2)+1;
    errorTable[1] = new char[strsize];
    strcpy(errorTable[1], temp2);
    strsize = strlen(temp3)+1;
    errorTable[2] = new char[strsize];
    strcpy(errorTable[2], temp3);
    strsize = strlen(temp4)+1;
    errorTable[3] = new char[strsize];
    strcpy(errorTable[3], temp4);
    strsize = strlen(temp5)+1;
    errorTable[4] = new char[strsize];
    strcpy(errorTable[4], temp5);
    strsize = strlen(temp6)+1;
    errorTable[5] = new char[strsize];
    strcpy(errorTable[5], temp6);

    const char* file0 = "main";
    const char* file1 = "common";
    const char* file2 = "memory";
    const char* file3 = "alu";
    const char* file4 = "control";
    const char* file5 = "op table";

    strsize = strlen(file0)+1;
    fileTable[0] = new char[strsize];
    strcpy(fileTable[0], file0);
    strsize = strlen(file1)+1;
    fileTable[1] = new char[strsize];
    strcpy(fileTable[1], file1);
    strsize = strlen(file2)+1;
    fileTable[2] = new char[strsize];
    strcpy(fileTable[2], file2);
    strsize = strlen(file3)+1;
    fileTable[3] = new char[strsize];
    strcpy(fileTable[3], file3);
    strsize = strlen(file4)+1;
    fileTable[4] = new char[strsize];
    strcpy(fileTable[4], file4);
    strsize = strlen(file5)+1;
    fileTable[5] = new char[strsize];
    strcpy(fileTable[5], file5);
}


//================================================================================== 
//Description: deconstructor
//================================================================================== 
ErrorTable::~ErrorTable()
{

}


//================================================================================== 
//Description: prints an error that is passed in and a file from where the error 
// was found
//================================================================================== 
void ErrorTable::printError(short error, short fileCode)
{
    short iErr = abs(error);

    if((iErr > 0) && (iErr < ERROR_MAX) && (fileCode > 0) 
                  && (fileCode < FILE_MAX) && (errorsOn))
    {
        if(errorsOn) //if silent mode
        {
            fprintf(stderr, "%s\n", PRINT_BREAK);
            fprintf(stderr, "***Error found in %s: %s***\n", fileTable[fileCode], errorTable[iErr]);
            fprintf(stderr, "%s\n", PRINT_BREAK);
        }
    }
}


//================================================================================== 
//Description: turns on debug messages
//================================================================================== 
void ErrorTable::debugOn(bool bSwitch)
{
    errorsOn = bSwitch;
}

