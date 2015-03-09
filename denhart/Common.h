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

#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

//Dependencies
//================================================================================== 
class BitReg;

//Defines
//================================================================================== 
#define DEBUG_COMMON true
#define REG_12BIT 12
#define REG_9BIT 9
#define REG_7BIT 7
#define REG_5BIT 5
#define REG_4BIT 4
#define REG_3BIT 3
#define REG_1BIT 1
#define INST_FORM_MRI "Memory Reference"
#define INST_FORM_OP "Micro Instruction"
#define INST_FORM_IO "Test I/O"
#define ADDRESS_LENGTH_OCT 4
#define INCREMENT_WORD 1
#define OPCODE_LENGTH 3
#define OPCODE_MIN 0
#define OPCODE_MRI 5
#define OPCODE_IO 6
#define OPCODE_OPP 7
#define OPCODE_MAX 7
#define OPCODE_MIN_INDEX 0
#define OPCODE_MAX_INDEX 2
#define INST_INDIRECT_BIT 3
#define INST_MEMPAGE_BIT 4
#define OFFSET_LENGTH 7
#define OFFSET_MIN_INDEX 5
#define OFFSET_MAX_INDEX 11
#define OFFSET_EXT_MIN_INDEX 3
#define OFFSET_EXT_LENGTH 9
#define OFFSET_PC_LENGTH 5
#define OFFSET_PC_MIN_INDEX 0
#define OFFSET_PC_MAX_INDEX 4
#define OPCODE_AND 0
#define OPCODE_TAD 1
#define OPCODE_ISZ 2
#define OPCODE_DCA 3
#define OPCODE_JMS 4
#define OPCODE_JMP 5
#define OPCODE_STR_AND "AND"
#define OPCODE_STR_TAD "TAD"
#define OPCODE_STR_ISZ "ISZ"
#define OPCODE_STR_DCA "DCA"
#define OPCODE_STR_JMS "JMS"
#define OPCODE_STR_JMP "JMP"
#define OPCODE_STR_IO "<IO>"
#define OPCODE_STR_OP "uInstructions"
#define START_ADDRESS 200


//file codes
#define FILE_MAIN 0
#define FILE_COMMON 1
#define FILE_CONV 2
#define FILE_BITREG 3
#define FILE_MEMORY 4
#define FILE_ALU 5
#define FILE_CONTROL 6
#define FILE_OPTABLE 8
#define FILE_MAX 9

//errors
#define ERROR_NONE 0
#define ERROR_CONV_FAILED -1
#define ERROR_NULL -2
#define ERROR_OUT_OF_RANGE -3
#define ERROR_UNEXPECTED_VALUE -4
#define ERROR_FILE -5
#define ERROR_DECONSTRUCTOR -6
#define ERROR_MAX 7 //max is positve to allocate errorTable array

//printing headers
#define PRINT_BREAK "-----------------------------------------------------"

//Constants
//================================================================================== 
const int ERRSIZE = ERROR_MAX;
const int FILESIZE = FILE_MAX;
const int MAX_BUFFER = 100;


//Class ErrorTable
//================================================================================== 
class ErrorTable
{
private:
    bool errorsOn;
    char* errorTable[ERRSIZE];
    char* fileTable[FILESIZE];

public:
    ErrorTable();
    ~ErrorTable();
    void printError(int error, int fileCode);
    void debugOn(bool bSwitch);
};


//Class RegTable
//================================================================================== 
class RegTable
{
private:

public:
    //Reading and writing memory
    //to read memory, a 12 bit address is loaded into the MA and
    //a read signal is asserted. The contents appear in the MB
    BitReg* rMA; //12 bit central processor memory address register
    BitReg* rMB; //12 bit Memory Buffer register
    //CPU registers
    BitReg* rAC; //12 bit accumulator
    BitReg* rLB; // 1 bit link bit (carry out)
    //BitReg* rMQ; //12 bit multiplier quotient
    BitReg* rPC; //12 bit program counter, holds address next instruction
    BitReg* rIR;  //3 bit instruction register, holds opcode of current instruction
    //BitReg* rSR;  //12 bit console switch register, used to load memory from console
    
    RegTable();
    ~RegTable();
    void incrementPC();
};


#endif //__COMMON_H_INCLUDED__

