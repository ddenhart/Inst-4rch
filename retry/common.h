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

//Defines
//================================================================================== 
#define INPUT_BINARY 0
#define INPUT_OCTAL 1
#define INPUT_HEX 2
#define REG_12BIT 12
#define REG_8BIT 8
#define MAX_OCT_ADDRESS 07777
#define MAX_DEC_ADDRESS 4095
#define ADDRESS_LENGTH_OCT 4
#define ADDRESS_LENGTH_HEX 3
#define ADDRESS_LENGTH_BIN 12
#define INDIRECT_MASK 256
#define MEMPAGE_MASK 128
#define IR_MASK 3584
#define OFFSET_MASK 127
#define OFFSET_PC_MASK 3968
#define OFFSET_AUTOINC_MIN 8
#define OFFSET_AUTOINC_MAX 15
#define OP_TABLE_LENG 8
#define OPCODE_AND 0
#define OPCODE_TAD 1
#define OPCODE_ISZ 2
#define OPCODE_DCA 3
#define OPCODE_JMS 4
#define OPCODE_JMP 5
#define OPCODE_IO 6
#define OPCODE_MICRO 7
#define HALT_CODE 3842
#define INST_FORM_MRI "Memory Reference\0"
#define INST_FORM_OP "Micro Instruction\0"
#define INST_FORM_IO "Test I/O\0"
#define BIT0_MASK 2048
#define BIT1_MASK 1024
#define BIT2_MASK 512
#define BIT3_MASK 256
#define BIT4_MASK 128
#define BIT5_MASK 64
#define BIT6_MASK 32
#define BIT7_MASK 16
#define BIT8_MASK 8
#define BIT9_MASK 4
#define BIT10_MASK 2
#define BIT11_MASK 1
#define REG_12BIT_MASK 4095
#define REG_6BIT_MASK 63
#define SYMBOLS_MAX 16
#define REG_UPPER 6
#define OPSTRING_AND "AND\0"
#define OPSTRING_TAD "TAD\0"
#define OPSTRING_ISZ "ISZ\0"
#define OPSTRING_DCA "DCA\0"
#define OPSTRING_JMS "JMS\0"
#define OPSTRING_JMP "JMP\0"
#define OPSTRING_IO "IO \0"
#define OPSTRING_MICRO "MIC\0"



//printing headers
#define PRINT_BREAK "-----------------------------------------------"

//file codes
#define FILE_MAIN 0
#define FILE_COMMON 1
#define FILE_MEM 2
#define FILE_ALU 3
#define FILE_CONTROL 4
#define FILE_OPTABLE 5
#define FILE_MAX 6

//errors
#define ERROR_NONE 0
#define ERROR_CONV_FAILED -1
#define ERROR_NULL -2
#define ERROR_OUT_OF_RANGE -3
#define ERROR_UNEXPECTED_VALUE -4
#define ERROR_FILE -5
#define ERROR_MAX 6 //max is positive to allocate errorTable array

//Constants
//================================================================================== 
const short ERRSIZE = ERROR_MAX;
const short FILESIZE = FILE_MAX;
const short MAX_BUFFER = 100;

//Global Methods
//================================================================================== 


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
    void printError(short error, short fileCode);
    void debugOn(bool bSwitch);
};


#endif //__COMMON_H_INCLUDED__
