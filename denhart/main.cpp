/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    Main.cpp
Date:			    03/02/2015
Description:	 This file contains the main
================================================================================== */

#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include "Common.h"
#include "BitReg.h"
#include "OctConv.h"
#include "ControlUnit.h"

//External objects
//================================================================================== 
ErrorTable Error;
RegTable RegisterFile;
OctConv OctTable;

//Global constants
//================================================================================== 
const char* default_logfile = "pdp8log.txt";
const char* default_tracefile = "add01.as";

//Global Methods
//================================================================================== 
//void parseflags(int count, char** args);


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int main (int argc, char **argv)
{
    bool silent = false; //pass to error class
    bool bDebugOn = false;
    bool bHelp = false;
    bool bHex = false;
    bool bOct = false;
    bool bBin = false;
    bool bAbort = false;
    bool bInputFile = false;
    bool bOutputFile = false;
    ControlUnit controlUnit;
    char* infile = NULL;
    char* outfile = NULL;
    int mode = 0;

    /* Read command line arguments
    flags:
    [-h <help>]
    [-f <trace input file name>]
    [-d <debug mode>]
    [-m <memory output file name>]
    [-v | -o | -b <ASCII hexadecimal format> | <ASCII octal format> | <binary object file> ]
    */

    for(int i = 0; i < argc; ++i)
    {
        if(!strcmp(argv[i], "-h"))
        {
            fprintf(stdout, "Usage: ");
        }
        else if(!strcmp(argv[i], "-f"))
        {
            bInputFile = true;
            //get the next argument for the file name
            ++i;
            if(argv[i][0] == '-')
            {
                //missing file name
                bAbort = true;
            }
            else
            {
                infile = new char[strlen(argv[i])+1];
                strcpy(infile, argv[i]);
            }
        }
        else if(!strcmp(argv[i], "-m"))
        {
            bOutputFile = true;
            //get the next argument for the file name
            ++i;
            if(argv[i][0] == '-')
            {
                //missing file name
                bAbort = true;
            }
            else
            {
                outfile = new char[strlen(argv[i])+1];
                strcpy(outfile, argv[i]);
            }
        }
        else if(!strcmp(argv[i], "-d"))
        {
            //set the debug flag
            bDebugOn = true;
            Error.debugOn(bDebugOn); // Turn of option error messages
        }
        else if(!strcmp(argv[i], "-v"))
        {
            bHex = true;
        }
        else if(!strcmp(argv[i], "-o"))
        {
            bOct = true;
        }
    }

    if(infile)
    {
        if((bHex && !bOct && !bBin) ||
           (!bHex && bOct && !bBin))
        {
            if(bHex)
            {
                mode = INPUT_HEX;
            }
            else if(bOct)
            {
                mode = INPUT_OCTAL;
            }
        }
        else if(!bHex && !bOct && !bBin)
        {
            bBin = true;
            mode = INPUT_BINARY;
        }
        else
        {
            //error output: -o and -v can't both be set
            bAbort = true;
            fprintf(stdout, "Error: input can't have both --o and --v flags set\n");
        }

        if(!bInputFile)
        {
            infile = new char[strlen(default_tracefile)+1];
            strcpy(infile, default_tracefile);
        }
        

        if(!bOutputFile)
        {
            outfile = new char[strlen(default_logfile)+1];
            strcpy(outfile, default_logfile);
        }

    }
    else
    {
        bAbort = true;
    }
    if(!bAbort)
    {
        controlUnit.loadFile(infile, mode);
        controlUnit.printMemoryHistory(outfile);
    }

    if(infile)
    {
        delete [] infile;
        infile = NULL;
    }

    if(outfile)
    {
        delete[] outfile;
        outfile = NULL;
    }

    std::system("pause");

    return 0;
}

