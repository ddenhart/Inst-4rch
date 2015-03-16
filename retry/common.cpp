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
    strcpy_s(errorTable[0], strsize, temp1);
    strsize = strlen(temp2)+1;
    errorTable[1] = new char[strsize];
    strcpy_s(errorTable[1], strsize, temp2);
    strsize = strlen(temp3)+1;
    errorTable[2] = new char[strsize];
    strcpy_s(errorTable[2], strsize, temp3);
    strsize = strlen(temp4)+1;
    errorTable[3] = new char[strsize];
    strcpy_s(errorTable[3], strsize, temp4);
    strsize = strlen(temp5)+1;
    errorTable[4] = new char[strsize];
    strcpy_s(errorTable[4], strsize, temp5);
    strsize = strlen(temp6)+1;
    errorTable[5] = new char[strsize];
    strcpy_s(errorTable[5], strsize, temp6);

    const char* file0 = "main";
    const char* file1 = "common";
    const char* file2 = "memory";
    const char* file3 = "alu";
    const char* file4 = "control";
    const char* file5 = "op table";

    strsize = strlen(file0)+1;
    fileTable[0] = new char[strsize];
    strcpy_s(fileTable[0], strsize, file0);
    strsize = strlen(file1)+1;
    fileTable[1] = new char[strsize];
    strcpy_s(fileTable[1], strsize, file1);
    strsize = strlen(file2)+1;
    fileTable[2] = new char[strsize];
    strcpy_s(fileTable[2], strsize, file2);
    strsize = strlen(file3)+1;
    fileTable[3] = new char[strsize];
    strcpy_s(fileTable[3], strsize, file3);
    strsize = strlen(file4)+1;
    fileTable[4] = new char[strsize];
    strcpy_s(fileTable[4], strsize, file4);
    strsize = strlen(file5)+1;
    fileTable[5] = new char[strsize];
    strcpy_s(fileTable[5], strsize, file5);
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

