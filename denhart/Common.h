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

#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

//Dependencies
//================================================================================== 
class BitReg;

//Defines
//================================================================================== 
//#define DEBUG_COMMON 
#define REG_12BIT 12
#define REG_9BIT 9
#define REG_8BIT 8
#define REG_7BIT 7
#define REG_5BIT 5
#define REG_4BIT 4
#define REG_3BIT 3
#define REG_1BIT 1
#define INST_FORM_MRI "Memory Reference\0"
#define INST_FORM_OP "Micro Instruction\0"
#define INST_FORM_IO "Test I/O\0"
#define ADDRESS_LENGTH_OCT 4
#define ADDRESS_LENGTH_HEX 3
#define ADDRESS_LENGTH_BIN 12
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
#define START_ADDRESS 200
#define HALT_CODE 3842
#define INPUT_BINARY 0
#define INPUT_OCTAL 1
#define INPUT_HEX 2
#define INPUT_OCTBYTE_MAX 6
#define OPSTRING_AND "AND\0"
#define OPSTRING_TAD "TAD\0"
#define OPSTRING_ISZ "ISZ\0"
#define OPSTRING_DCA "DCA\0"
#define OPSTRING_JMS "JMS\0"
#define OPSTRING_JMP "JMP\0"
#define OPSTRING_IO "IO \0"
#define OPSTRING_MICRO "MIC\0"
#define OP_TABLE_LENG 8
#define OPSTRING7_CMA "CMA\0"
#define OPSTRING7_IAC "IAC\0"
#define OPSTRING7_CIA "CIA\0"
#define OPSTRING7_CLA "CLA\0"
#define OPSTRING7_HLT "HLT\0"
#define OPSTRING7_SMA "SMA\0"
#define OPSTRING7_SZA "SZA\0"
#define OPSTRING7_SPA "SPA\0"
#define OPSTRING7_SNA "SNA\0"
#define OPSTRING7_RAR "RAR\0"
#define OPSTRING7_RAL "RAL\0"
#define OPSTRING7_RTL "RTL\0"
#define OPSTRING7_RTR "RTR\0"
#define OPSTRING7_BSW "BSW\0"
#define OPSTRING7_CML "CML\0"
#define OPSTRING7_CLL "CLL\0"
#define OPSTRING7_STL "STL\0"
#define OPSTRING7_STA "STA\0"
#define OPSTRING7_OSR "OSR\0"
#define OPSTRING7_SKP "SKP\0"
#define OPSTRING7_SNL "SNL\0"
#define OPSTRING7_SZL "SZL\0"
#define OPSTRING7_NOP "NOP\0"
#define OP7_LIST_LENG 23
#define OP7_CMA_CML 7060 //complement the alu and link
#define OP7_CLA_CLL 7300 //clear the alu and link
#define OP7_CLA_IAC 7201 //set alu to 1


//file codes
#define FILE_MAIN 0
#define FILE_COMMON 1
#define FILE_CONV 2
#define FILE_BITREG 3
#define FILE_MEMORY 4
#define FILE_ALU 5
#define FILE_CONTROL 6
#define FILE_OPTABLE 7
#define FILE_MAX 8

//errors
#define ERROR_NONE 0
#define ERROR_CONV_FAILED -1
#define ERROR_NULL -2
#define ERROR_OUT_OF_RANGE -3
#define ERROR_UNEXPECTED_VALUE -4
#define ERROR_FILE -5
#define ERROR_MAX 6 //max is positve to allocate errorTable array

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

