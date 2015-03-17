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
#include <fstream>
#include <iostream>
#include <cstdio>
#include "common.h"
#include "controlunit.h"

class ErrorTable Error;
void pauseandexit();


int main(int argc, char **argv)
{
    const char* memdump = "pdp8log.txt\0";
    //bool silent = false; //pass to error class
    //bool bDebugOn = false;
    //bool bHelp = false;
    bool bHex = false;
    bool bOct = false;
    bool bBin = false;
    bool bAbort = false;
    ControlUnit controlUnit;
    char* infile = NULL;
    char* outfile = NULL;
    short mode = 0;
    int strsize = 0;

    /* Read command line arguments
    flags:
    [-h <help>]
    [-f <trace input file name>]
    [-d <debug mode>]
    [-m <memarray output file name>]
    [-v | -o | -b <ASCII hexadecimal format> | <ASCII octal format> | <binary object file> ]
    */

    for(short i = 0; i < argc; ++i)
    {
        //if(!strcmp(argv[i], "-h"))
        //{
        //    fprintf(stdout, "Usage: ");
        //}
        //else 
        if(!strcmp(argv[i], "-f"))
        {
            //bInputFile = true;
            //get the next argument for the file name
            ++i;
            if(argv[i][0] == '-')
            {
                //missing file name
                bAbort = true;
            }
            else
            {
                strsize = strlen(argv[i])+1;
                infile = new char[strsize];
                strcpy(infile, argv[i]);
            }
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
            fprintf(stderr, "Shutting down...\n ");
            pauseandexit();
        }
        strsize = strlen(memdump)+1;
        outfile = new char[strsize];
        strcpy(outfile, memdump);
    }
    else
    {
        bAbort = true;
    }
    if(!bAbort)
    {
        controlUnit.loadFile(infile, mode);
    }

    if(infile)
    {
        delete[] infile;
        infile = NULL;
    }

    if(outfile)
    {
        delete[] outfile;
        outfile = NULL;
    }

    fprintf(stderr, "Halt...\n ");
    pauseandexit();

    return 0;
}


void pauseandexit()
{
    //fprintf(stderr, "Shutting down...\n ");
    std::cin.sync(); // Flush The Input Buffer Just In Case
    std::cin.ignore(); // There's No Need To Actually Store The Users Input
    exit(EXIT_FAILURE);
}