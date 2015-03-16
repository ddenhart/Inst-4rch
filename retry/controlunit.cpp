/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke

==================================================================================
File:			    controlunit.cpp
Date:			    03/02/2015
Description:	 This file contains the classes InstFormat, EffectiveAddress and
ControlUnit 
================================================================================== */

#include <fstream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "Common.h"
#include "OpTable.h"
#include "memory.h"
#include "controlunit.h"
#include "alu.h"

//External objects
//================================================================================== 
extern ErrorTable Error;


//================================================================================== 
//Description:constructor
//================================================================================== 
ControlUnit::ControlUnit()
{
    silent = false;
    readfile = NULL;
    m_alu = new alu;
    m_mem = new memarray;
    m_eAddy.setmem(m_mem);
    m_StartAddress = 0200;
    running = true;
    initSymbols();
}


//================================================================================== 
//Description:deconstructor
//================================================================================== 
ControlUnit::~ControlUnit()
{
    //silent = false;
    if(m_mem)
    {
        delete m_mem;
        m_mem = NULL;
    }
    if(m_alu)
    {
        delete m_alu;
        m_alu = NULL;
    }
}


//================================================================================== 
//Description: Converts a 3 char hex value to a 4
//================================================================================== 
unsigned short ControlUnit::hexAddressHandle(char* addy)
{
    unsigned short reg = 0;

    for(int i = 0; i < ADDRESS_LENGTH_HEX; ++i)
    {
        reg += convToNumber(addy[i]);
        reg = reg << 4;
    }

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG convert hex input string to octal: %o\n", reg);
#endif

    return reg;
}


//================================================================================== 
//Description: parses a hex file
//================================================================================== 
void ControlUnit::modeHex(char* filename)
{
    char line[MAX_BUFFER]; // String for getline
    int length = 0;
    bool bFirstLine = true;
    unsigned short reg = 0;
    unsigned short rpc = 0;

    if(NULL != filename)
    {
        fopen_s(&readfile, filename, "r");
    }
    
    if(readfile)
    {
        while((!feof(readfile)) && (fgets(line, MAX_BUFFER, readfile)))
        {
            length = strlen(line);

            if(length == (ADDRESS_LENGTH_HEX+1))
            {
                //it's data
                reg = hexAddressHandle(line);
                rpc = getPC();

#ifdef DEBUG_CONTROL
                    fprintf(stdout, "DEBUG: read hex in data: addy: %o  data: %o\n", rpc, reg);
#endif
                m_mem->store(rpc, reg);
                incrementPC();

            }
            else if(length == (ADDRESS_LENGTH_OCT+1))
            {
                if('@' == line[0]) //address
                {
                    memmove(line, line+1, strlen(line)); //remove @
                    reg = hexAddressHandle(line);
#ifdef DEBUG_CONTROL
                    fprintf(stdout, "DEBUG: read hex in address: %o\n", reg);
#endif
                    if(bFirstLine)
                    {
                        m_StartAddress = reg;
                    }

                    bFirstLine = false;
                    setPC(reg);
                }
                else
                {
                    Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
                }
            }
            else
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
            }

            reg = 0;
        }

        fclose(readfile);
    }
}


//==================================================================================
//Description: parses a binary file
//==================================================================================
void ControlUnit::modeBin(char* filename)
{
    bool bFirstLine = true;
    unsigned short addyfound = 0;
    unsigned short buff = 0;
    unsigned short rpc = 0;
    unsigned short conv = 0;
    unsigned char* buffer = NULL;
    std::vector<char> sBuff;
    std::vector<char>::iterator it;

    if(NULL != filename)
    {
       fopen_s(&readfile, filename, "r+b");
    }

    if(readfile)
    {
        //get the size of the file
        fseek(readfile, 0, SEEK_END);
        long fsize = ftell(readfile);
        buffer = new unsigned char[fsize+1];
        rewind(readfile);

        //read the file
        fread(buffer, 1, fsize, readfile);
        fclose(readfile);
        buffer[fsize] = '\0';

        //get rid of headers
        for(short i = 0; i < fsize; ++i)
        {
            if(buffer[i] == 128)
            {
                //skip this character.. it's a buffer
            }
            else if(buffer[i] == '\a')
            {
                break; //stop at the bell
            }
            else
            {
                sBuff.push_back(buffer[i]);
            }
        }

        //parse the binary
        for(it = sBuff.begin(); it != sBuff.end(); ++it)
        {
            for(int i = 0; i < 2; ++i)
            {
                conv = convToNumber(*it);
                if(0 == i)
                {
                    addyfound = conv & BIT1_MASK;
                    buff = conv & REG_6BIT_MASK;
                    buff = buff << REG_UPPER;
                    ++it;
                    if(sBuff.end() == it)
                    {
#ifdef DEBUG_CONTROL
                        fprintf(stdout, "DEBUG: invalid binary format: %o\n", buff);
#endif
                        break;
                    }
                }
                else
                {
                    conv = conv & REG_6BIT_MASK;
                    buff += conv;
                }
            }

            if(addyfound) //its an address
            {
                buff = m_mem->checkValidAddy(buff);
                if(bFirstLine)
                {
                    m_StartAddress = buff;
                }

                bFirstLine = false;
                setPC(buff);
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG: read binary in data: addy: %o  data: %o\n", rpc, buff);
#endif
            }
            else //its data
            {
                rpc = getPC();
                m_mem->store(rpc, buff);
                incrementPC();
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG: read in binary address: %o\n", buff);
#endif
            }

            buff = 0;
        }
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
    }
}


//================================================================================== 
//Description:  parses an octal file
//================================================================================== 
void ControlUnit::modeOct(char* filename)
{
    char line[MAX_BUFFER]; // String for getline
    unsigned char sInput[ADDRESS_LENGTH_OCT + 1] = {'\0', '\0', '\0', '\0', '\0'};
    bool bAddy = false;
    bool bFirstLine = true;
    int pair = 0;
    unsigned short conv = 0;
    unsigned short buff = 0;
    unsigned short rpc = 0;
    short length = 0;
    std::vector<char> sBuff;
    std::vector<char>::iterator it;

    fopen_s(&readfile,filename, "r");

    if(readfile)
    {
        while(!feof(readfile) && fgets(line, MAX_BUFFER, readfile))
        {
            length = strlen(line);
            if(length <= ADDRESS_LENGTH_OCT) // expected format including \0
            {
                if(0 == pair)
                {
                    if('1' == line[0])
                    {
                        bAddy = true;
                    }
                    else if('0' == line[0])
                    {
                        bAddy = false;
                    }
                    else
                    {
                        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
                    }
                }

                ++pair;
                if(length == ADDRESS_LENGTH_OCT)
                {
                    sBuff.push_back(line[1]);
                    sBuff.push_back(line[2]);
                }
                else if(length == (ADDRESS_LENGTH_OCT-1))
                {
                    sBuff.push_back('0');
                    sBuff.push_back(line[2]);
                }
                else
                {
                    Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
                }
            }
            else
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
            }

            if(2 == pair)
            {
                //parse the binary
                for(it = sBuff.begin(); it != sBuff.end(); ++it)
                {
                    buff = buff << 3;
                    conv = convToNumber(*it);
                    buff += conv;
                }

                if(bAddy) //it's an address
                {
                    buff = m_mem->checkValidAddy(buff);  //check for valid address
                    if(bFirstLine)
                    {
                        m_StartAddress = buff;
                    }

                    bFirstLine = false;
                    setPC(buff);
#ifdef DEBUG_CONTROL
                    fprintf(stdout, "DEBUG: read in octal address: %o\n", buff);
#endif
                }
                else //it's data
                {
                    rpc = getPC();
                    m_mem->store(rpc, buff);
                    incrementPC();
#ifdef DEBUG_CONTROL
                    fprintf(stdout, "DEBUG: read octal in data: addy: %o  data: %o\n", rpc, buff);
#endif
                }
                    pair = 0;
                    buff = 0;
                    sBuff.clear();
            }
        }

        fclose(readfile);
    }
}


//================================================================================== 
//Description: reads the new address from memory
//================================================================================== 
unsigned short ControlUnit::readData(unsigned short address)
{
    m_mem->load(address);
    unsigned short temp = m_mem->readMB();

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: reading instruction from memory: address: %o data: %o\n", address, temp);
#endif

    return temp;
}


//================================================================================== 
//Description: fetches an instruction from memory located at rPC and increments rPC
//================================================================================== 
void ControlUnit::instructionFetch()
{
    //contents of the PC are loaded short to the MA
    unsigned short reg = 0;
    m_mem->load(rPC);
    reg = m_mem->readMB();
    m_mem->fetch(rPC);
    m_format.loadInstruction(reg);
#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: fetched: data: %o\n", reg);
#endif
}


//================================================================================== 
//Description: muxes the opcode, retrieves indirect addresses if necessary and
// sends results to be executed
//================================================================================== 
void ControlUnit::instructionDecode()
{
    unsigned short ops = 0;
    unsigned short addy = 0;
    unsigned short data = 0;
    unsigned short currInst = 0;

    ops = m_format.getOpcode(); 

    //up cycle count
    m_format.incrementCycles(ops);
    currInst = m_format.getInstruction();

    if(m_format.isInstMRI())
    {
        addy = m_eAddy.geteffAddress(currInst, rPC); //get the address
        instructionDefer();
        data = readData(addy);
        executeMRI(addy, data);

#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: mri decode: address: %o, data: %o\n", addy, data);
#endif
    }
    else if(m_format.isInstOperate())
    {
        //micro setup
        executeMicro(currInst);

#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: micro decode: opcode: %o\n", currInst);
#endif
    }
    else if(m_format.isInstTestIO())
    {
        //IO setup
#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: IO decode: nops\n");
#endif
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }
}


//==================================================================================
//Description: checks if the instruction is indirect and retrieves the new address,
// incrementing the number of necessary cycles
//================================================================================== 
void ControlUnit::instructionDefer()
{
    unsigned short ops = 0;
    //if indirect, costs 2 cycles
    if(m_eAddy.isIndirect())
    {
        ops = m_format.getOpcode();
        //up cycle count
        m_format.incrementCyclesDefer(ops);
#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: defer: opcode %o\n", ops);
#endif
    }
}


//================================================================================== 
//Description: returns the value of rPC
//================================================================================== 
unsigned short ControlUnit::getPC()
{
    return rPC;
}


//================================================================================== 
//Description: checks addy to make sure it is valid and sets rPC to addy
//================================================================================== 
void ControlUnit::setPC(unsigned short addy)
{
    addy = m_mem->checkValidAddy(addy);
    rPC = addy;
}


//================================================================================== 
//Description: executes an MRI instruction
//================================================================================== 
void ControlUnit::executeMRI(unsigned short addy, unsigned short data)
{
    unsigned short temp = 0;
    unsigned short opcode = m_format.getOpcode();
    bool skipIncrement = false;

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: executing mri: opcode %o\n", opcode);
#endif

    if(OPCODE_AND == opcode)
    {
        m_alu->andALU(data);
    }
    else if(OPCODE_TAD == opcode)
    {
        m_alu->sumALU(data);
    }
    else if(OPCODE_ISZ == opcode)
    {
        temp = data + 1; //increment the data
        m_mem->store(addy, temp); //store back in memory
        if(temp == 0)
        {
            incrementPC();
        }
    }
    else if(OPCODE_DCA == opcode)
    {
        //store result of alu
        m_mem->store(addy, m_alu->getAC());
        m_alu->clear();
    }
    else if(OPCODE_JMS == opcode)
    {
        m_mem->store(addy, rPC); //store the pc in the current address
        addy += 1; //increment the address
        addy = m_mem->checkValidAddy(addy);
        setPC(addy); //set pc to the new addy
    }
    else if(OPCODE_JMP == opcode)
    {
        addy = m_mem->checkValidAddy(addy);
        setPC(addy); // set pc to the new addy
        skipIncrement = true;
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }
        
    if(!skipIncrement)
    {
        incrementPC();
    }
}


//==================================================================================
//Description: executes the micro instructions
//==================================================================================
void ControlUnit::executeMicro(unsigned short inst)
{
    unsigned short bit3 = inst & BIT3_MASK;
    bit3 = bit3 >> 8;
    unsigned short bit4 = inst & BIT4_MASK;
    bit4 = bit4 >> 7;
    unsigned short bit5 = inst & BIT5_MASK;
    bit5 = bit5 >> 6;
    unsigned short bit6 = inst & BIT6_MASK;
    bit6 = bit6 >> 5;
    unsigned short bit7 = inst & BIT7_MASK;
    bit7 = bit7 >> 4;
    unsigned short bit8 = inst & BIT8_MASK;
    bit8 = bit8 >> 3;
    unsigned short bit9 = inst & BIT9_MASK;
    bit9 = bit9 >> 2;
    unsigned short bit10 = inst & BIT10_MASK;
    bit10 = bit10 >> 1;
    unsigned short bit11 = inst & BIT11_MASK;

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: executing micro: instruction: %o\n", inst);
#endif

    if(bit3 == 0)  //Group 1
    {
        if(bit4 == 1)
        {
            m_alu->clearAC();
        }
        if(bit5 == 1)
        {
            m_alu->clearLC();
        }
        if(bit6 == 1)
        {
            m_alu->complementALU();
        }
        if(bit7 == 1)
        {
            m_alu->complementLC();
            if(bit11 == 1)
                m_alu->incrementALU();
        }
        if(bit8 == 1)
        {
            m_alu->rotateRight();
            if(bit10 == 1)
                m_alu->rotateRight();
        }
        if(bit9 == 1)
        {
            m_alu->rotateLeft();
            if(bit10 == 1)
                m_alu->rotateLeft();
        }
    }
    else
    {
        if(bit8 == 0)
        {
            if((bit5 == 1) || (bit6 == 1) || (bit7 == 1))
            {
                if(bit5 == 1)
                {
                    if(m_alu->isNegative())
                        incrementPC();
                }
                if(bit6 == 1)
                {
                    if(m_alu->isZero())
                        incrementPC();
                }
                if(bit7 == 1)
                {
                    if(m_alu->getLB() != 0)
                        incrementPC();
                }

            }
        }
        else
        {
            short skip = false;
            /*if ( ( (bit5 == 1) && (bit6 == 1) ) 
                || ( (bit5 == 1) && (bit7 == 1) )
                || ( (bit6 == 1) && (bit7 == 1) ) )
            {
                incrementPC();
            }*/
            if(bit5 == 1)
            {
                skip = false;
                if(!m_alu->isNegative())
                {
                    skip = true;
                }
            }
            if(bit6 == 1)
            {
                skip = false;
                if(!m_alu->isZero())
                {
                    skip = true;
                }
            }
            if(bit7 == 1)
            {
                skip = false;
                if(m_alu->getLB() == 0)
                {
                    skip = true;
                }
            }
            if(skip)
            {
                incrementPC();
            }
            if((bit4 == 0) && (bit5 == 0)
               && (bit6 == 0) && (bit7 == 0)
               && (bit8 == 1) && (bit9 == 0)
               && (bit10 == 0) && (bit11 == 0))
            {
                incrementPC();
            }
            if(bit4 == 1)
            {
                m_alu->clearAC();
                running = false;
            }
        }
    }
    incrementPC();
}


//==================================================================================
//Description: loads the input file and parses flags
//==================================================================================
void ControlUnit::loadFile(char* filename, short mode)
{
    unsigned short rpc = 0;
    unsigned short data = 0;
    short length = 0;
    bool bMemValid = true;


    if(INPUT_BINARY == mode)
    {
        modeBin(filename);
    }
    else if(INPUT_HEX == mode)
    {
        modeHex(filename);
    }
    else if(INPUT_OCTAL == mode)
    {
        modeOct(filename);
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }

    m_mem->writeMemoryAccesses();
    setPC(m_StartAddress);
    while(HALT_CODE != m_mem->readMB())
    {
        instructionFetch();
        instructionDecode();
        rpc = m_mem->checkValidAddy(rpc);
        if(!bMemValid)
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
            //throw error if memarray access is out of bounds
            //fprintf(stderr, "%s\n", PRINT_BREAK);
            //throw fprintf(stderr, "Error: Exceeded memarray space...\n");
            //fprintf(stderr, "%s\n", PRINT_BREAK);
            //return;
        }
    }
    m_format.printStats();
}


//================================================================================== 
//Description: writes all memarray accesses to file
//================================================================================== 
void ControlUnit::printMemoryHistory()
{
    if(m_mem)
    {
        m_mem->writeMemoryAccesses();
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}

//================================================================================== 
//Description: checks addy to make sure it is valid and sets rPC to addy
//================================================================================== 
void ControlUnit::incrementPC()
{
    ++rPC;
    rPC = m_mem->checkValidAddy(rPC);
}


//================================================================================== 
//Description: initialize symbol table
//================================================================================== 
void ControlUnit::initSymbols()
{
    m_symbolTable[0] = '0';
    m_symbolTable[1] = '1';
    m_symbolTable[2] = '2';
    m_symbolTable[3] = '3';
    m_symbolTable[4] = '4';
    m_symbolTable[5] = '5';
    m_symbolTable[6] = '6';
    m_symbolTable[7] = '7';
    m_symbolTable[8] = '8';
    m_symbolTable[9] = '9';
    m_symbolTable[10] = 'A';
    m_symbolTable[11] = 'B';
    m_symbolTable[12] = 'C';
    m_symbolTable[13] = 'D';
    m_symbolTable[14] = 'E';
    m_symbolTable[15] = 'F';
}



//==================================================================================
//Description: finds a hex equivalent for the input symbolic char
//==================================================================================
short ControlUnit::convToNumber(char sIn)
{
    short i = 0;
    short res = -1;
    char conv = toupper(sIn);

    for(i = 0; i < SYMBOLS_MAX; ++i)
    {
        if(conv == m_symbolTable[i])
        {
            res = i;
            break;
        }
    }

    return res;
}


//================================================================================== 
//Description: constructor
//================================================================================== 
InstFormat::InstFormat()
{
    m_opTable = new optable;
    reset();
}


//================================================================================== 
//Description:deconstructor
//================================================================================== 
InstFormat::~InstFormat()
{
    if(m_opTable)
    {
        delete m_opTable;
        m_opTable = NULL;
    }
}


//================================================================================== 
//Description: resets all saved addresses and data
//================================================================================== 
void InstFormat::reset()
{
    m_bMRI = false;
    m_bOperate = false;
    m_bTestIO = false;
    m_iMicroCode = 0;
}


//================================================================================== 
//Description:extracts the opcode from the instruction and sets it to the rIR
//================================================================================== 
void InstFormat::setIR(unsigned short inst)
{
    rIR = inst & IR_MASK;
    rIR = rIR >> 9;
#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: set IR: %o\n", rIR);
#endif
}


//================================================================================== 
//Description: returns the contents of the IR register
//================================================================================== 
unsigned short InstFormat::getOpcode()
{
    return rIR;
}


//================================================================================== 
//Description: sets the type of instruction format
//================================================================================== 
void InstFormat::setInstType()
{
    if(OPCODE_JMP >= rIR)
    {
        m_bMRI = true;
        m_bTestIO = false;
        m_bOperate = false;
    }
    else if(OPCODE_IO == rIR)
    {
        m_bMRI = false;
        m_bTestIO = true;
        m_bOperate = false;
    }
    else if(OPCODE_MICRO == rIR)
    {
        m_bMRI = false;
        m_bTestIO = false;
        m_bOperate = true;
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }
}


//================================================================================== 
//Description:loads the current instruction
//================================================================================== 
void InstFormat::loadInstruction(unsigned short inst)
{
    reset();
    m_rInstruction = inst;
    setIR(inst);
    setInstType();
    m_iMicroCode = inst;


#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: format loading instruction: %o\n", inst);
#endif
}


//================================================================================== 
//Description:returns the type of format in a string 
//================================================================================== 
char* InstFormat::getInstType()
{
    char* temp = NULL;
    char* stemp1 = INST_FORM_MRI;
    char* stemp2 = INST_FORM_OP;
    char* stemp3 = INST_FORM_IO;
    int strsize = 0;

    if(m_bMRI)
    {
        strsize = strlen(stemp1)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, stemp1);
    }
    else if(m_bOperate)
    {
        strsize = strlen(stemp2)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, stemp2);
    }
    else if(m_bTestIO)
    {
        strsize = strlen(stemp3)+1;
        temp = new char[strsize];
        strcpy_s(temp, strsize, stemp3);
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }

    return temp;
}


//================================================================================== 
//Description:returns the micro instruction
//================================================================================== 
unsigned short InstFormat::getInstruction()
{
    return m_iMicroCode;
}


//================================================================================== 
//Description:checks if instruction is a MRI format
//================================================================================== 
bool InstFormat::isInstMRI()
{
    return m_bMRI;
}


//==================================================================================
//Description:checks if instruction is a OPP format
//================================================================================== 
bool InstFormat::isInstOperate()
{
    return m_bOperate;
}


//================================================================================== 
//Description:checks if instruction is an IO format
//================================================================================== 
bool InstFormat::isInstTestIO()
{
    return m_bTestIO;
}


//================================================================================== 
//Description: increments cycles for MRI
//================================================================================== 
void InstFormat::incrementCycles(unsigned short ops)
{
    if(ops < OP_TABLE_LENG)
    {
        m_opTable->incrementOpsCycle(ops);
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
    }
}


//================================================================================== 
//Description: increments a cycle if the instruction is deferred
//================================================================================== 
void InstFormat::incrementCyclesDefer(unsigned short ops)
{
    if(ops < OP_TABLE_LENG)
    {
        m_opTable->incrementOneCycle(ops);
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
    }
}


//================================================================================== 
//Description:prints the stat summary for all instructions
//================================================================================== 
void InstFormat::printStats()
{
    m_opTable->printUsageAndCycles();
}




//================================================================================== 
//Description:constructor
//================================================================================== 
EffectiveAddress::EffectiveAddress()
{
    m_rpc = 0; //copy of the current pc
    m_rOffset = 0; //7 bit offset
    m_rZeroPage = 0; //5 bit, the first memarray page
    m_rCurrPage = 0;  //5 bit, the current page
    m_rResultAddress = 0; //12 bit resulting address
    m_indirect = false;
    m_pmem = NULL;
}


//================================================================================== 
//Description: deconstructor
//================================================================================== 
EffectiveAddress::~EffectiveAddress()
{
}


//================================================================================== 
//Description: sets the memory pointer
//================================================================================== 
void EffectiveAddress::setmem(memarray* mem)
{
    if(mem)
    {
        m_pmem = mem;
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}


//================================================================================== 
//Description: set the lower six bytes of the pc to the offset of the current page
//================================================================================== 
void EffectiveAddress::setPCOffset(unsigned short rpc)
{
    m_rCurrPage = rpc & OFFSET_PC_MASK;

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: effAddy: pc offset: %o\n", m_rCurrPage);
#endif
}


//==================================================================================
//Description:if the memarray page bit is 0, calculate 00000 + offset, 
//              range is 0000o-0177o
//================================================================================== 
unsigned short EffectiveAddress::effAdzeroPage()
{
    unsigned short temp = 0;

    //add page 0 to the offset
    temp = temp | m_rOffset;

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: effAddy: zero page: %o\n", temp);
#endif

    return temp;
}


//================================================================================== 
//Description: if the memarray page bit is 1, calculate instruction page + offset, 
//                  range instruction page
//================================================================================== 
unsigned short EffectiveAddress::effAdcurrentPage()
{
    unsigned short temp = 0;

    temp = m_rCurrPage | m_rOffset;

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: effAddy: current page: %o\n", temp);
#endif

    return temp;
}


//================================================================================== 
//Description:if the indirect bit is 1, calculate  c(address), 
//                  range is any address but costs 2 mem reads
//================================================================================== 
unsigned short EffectiveAddress::effAdindirectAddressZero()
{
    unsigned short temp = 0;

    temp = effAdzeroPage(); //get the effective address

    //auto index
    //calculate c(0010o - 0017o) + 1 effective address
    if((temp > OFFSET_AUTOINC_MIN) && (temp <= OFFSET_AUTOINC_MAX))
    {
        temp += 1;

#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: effAddy: auto incremented: %o\n", temp);
#endif
    }

    //normal zero page indirect
    temp = readAddress(temp); //read the memory contents of the address
    loadOffset(temp); //load the address to the offset
    temp = effAdzeroPage(); //recalcuate the new effective address

    return temp;
}


//================================================================================== 
//Description:if the indirect bit is 1, calculate  c(address), 
//              range is any address but costs 2 mem reads
//================================================================================== 
unsigned short EffectiveAddress::effAdindirectAddressCurr()
{
    unsigned short temp = 0;

    temp = effAdcurrentPage(); //get the effective address
    temp = readAddress(temp); //read the memory contents of the address
    loadOffset(temp); //load the address to the offset
    temp = effAdcurrentPage(); //recalcuate the new effective address

    return temp;
}


//================================================================================== 
//Description: extracts the offset from an MRI instruction
//================================================================================== 
void EffectiveAddress::loadOffset(unsigned short reg)
{
    m_rOffset = 0;

    if(m_pmem)
    {
        m_rOffset = reg & OFFSET_MASK;

#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: effAddy: offset: %o\n", m_rOffset);
#endif
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}


//================================================================================== 
//Description: reads the new address from memory
//================================================================================== 
unsigned short EffectiveAddress::readAddress(unsigned short address)
{
    m_pmem->load(address);
    return m_pmem->readMB();
}


//================================================================================== 
//Description: returns true if the instruction was indirect
//================================================================================== 
bool EffectiveAddress::isIndirect()
{
    return m_indirect;
}


//================================================================================== 
//Description:inputs an MRI instruction and returns the effective address
//================================================================================== 
unsigned short EffectiveAddress::geteffAddress(unsigned short reg, unsigned short rpc)
{
    unsigned short temp = 0;
    unsigned short indirect = reg & INDIRECT_MASK;
    indirect = indirect >> 8;
    unsigned short currpage = reg & MEMPAGE_MASK;
    currpage = currpage >> 7;

    loadOffset(reg);
    setPCOffset(rpc);
    if(indirect == 1)
    {
        m_indirect = true;
    }
    else
    {
        m_indirect = false;
    }

#ifdef DEBUG_CONTROL
    fprintf(stdout, "DEBUG: effAddy: indirect: %o \n", m_indirect);
#endif

    //zero page offset
    if(!currpage && !indirect)
    {
        temp = effAdzeroPage();
    }
    //zero page indirect
    else if(!currpage && indirect)
    {
        //auto indexing handled inside here
        temp = effAdindirectAddressZero();
    }
    //current page, no indirect
    else if(currpage && !indirect)
    {
        temp = effAdcurrentPage();
    }
    //current page and indirect
    else if(currpage && indirect)
    {
        temp = effAdindirectAddressCurr();
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }

    return temp;
}

