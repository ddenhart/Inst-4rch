/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    ControlUnit.cpp
Date:			    03/02/2015
Description:	 This file contains the classes InstFormat, EffectiveAddress,
                ControlUnit and Statistics
================================================================================== */

#include <fstream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include "Common.h"
#include "BitReg.h"
#include "OpTable.h"
#include "Memory.h"
#include "ControlUnit.h"

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
ControlUnit::ControlUnit()
{
    m_memory = new Memory;
    m_eAddy.setMemory(m_memory);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
ControlUnit::~ControlUnit()
{
    if(m_memory)
    {
        delete m_memory; 
        m_memory = NULL;
    }
    /*if(m_rCurrInst)
    {
        delete m_rCurrInst;
        m_rCurrInst = NULL;
    }
    if(m_rCurrAddy)
    {
        delete m_rCurrAddy;
        m_rCurrAddy = NULL;
    }*/
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ControlUnit::instructionFetch(BitReg* reg)
{
    //contents of the PC are loaded into the MA
    if(reg)
    {
        m_memory->fetch(reg);
        m_format.loadInstruction(reg);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ControlUnit::instructionDecode()
{
    BitReg* inst = NULL;
    BitReg* indinst = NULL;
    BitReg* currInst = NULL;

    currInst = m_format.getInstruction();

    if(m_format.isInstMRI())
    {
        inst = m_eAddy.getAddress(currInst);
        
        if(m_format.getMRIindirect())
        {
            indinst = inst->getReg();
            delete inst;
            inst = NULL;
            instructionDefer();
            inst = m_eAddy.getAddress(indinst);
        }
        
        m_format.setAddress(inst);
    }
    else if(m_format.isInstOperate())
    {
        //micro setup
    }
    else if(m_format.isInstTestIO())
    {
        //IO setup
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }

    if(inst)
    {
        delete inst;
        inst = NULL;
    }
    if(indinst)
    {
        delete inst;
        inst = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ControlUnit::instructionDefer()
{
    //if indirect, costs 2 cycles
    if(m_format.getMRIindirect())
    {
        //up cycle count
        RegisterFile.incrementPC();
        fprintf(stdout, "DEBUG: Defer: extra cycle");
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
BitReg* ControlUnit::getPC()
{
    return RegisterFile.rPC->getReg();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ControlUnit::setPC(BitReg* addy)
{
    return RegisterFile.rPC->setReg(addy);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ControlUnit::instructionExecute()
{
    BitReg* addy = NULL;
    BitReg* rpc = NULL;
    int opcode = m_format.getOpcode();

    addy = m_format.getAddress();
    rpc = getPC();

    if(addy && rpc)
    {
        if(0 == opcode)
        {
            //test code
            m_memory->load(addy);
            m_memory->load(rpc);
        }
        else if(1 == opcode)
        {
            //test code
            m_memory->load(addy);
            m_memory->load(rpc);
        }
        else if(2 == opcode)
        {
            //test code
            m_memory->load(addy);
            m_memory->load(rpc);
        }
        else if(3 == opcode)
        {
            //test code
            //store result of accumulator
            m_memory->store(addy, rpc);
            m_memory->load(rpc);

        }
        else if(4 == opcode)
        {
            //test code
            m_memory->load(addy);
            m_memory->load(rpc);

        }
        else if(5 == opcode)
        {
            //test code
            m_memory->load(addy);
            m_memory->load(rpc);

        }
        else if(6 == opcode)
        {
            //noop
            fprintf(stdout, "DEBUG: Execute: NOP\n");
        }
        else if(7 == opcode)
        {
            //opcode 7 micro instructions here
        }
        else
        {
            Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
        }

        RegisterFile.incrementPC();
        fprintf(stdout, "DEBUG: Execute: %s  %s  PC: %s\n", 
                m_format.getInstType(), addy->getString(), rpc->getString());
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
// Load  from file
// INPUT: file name to load from
void ControlUnit::load_from_file(char* filename)
{
    FILE* file = NULL;//(filename, std::ios::in);  // Open filename as file
    char line[MAX_BUFFER];               // String for getline
    //std::string line;               // String for getline
    char sInput[ADDRESS_LENGTH_OCT + 1];
    int maxGroup = 2;
    int maxOctLeng = 4;
    bool bPairFound = false;
    bool bAddy = false;
    std::vector<char*> buffer;
    BitReg rInput(REG_12BIT);     // Current address 
    BitReg rData(REG_12BIT);     // Current address 
    unsigned int data = 0;
    int length = 0;

    file = fopen(filename, "r");

    if(file)
    {
        while(!feof(file))
        {
            fgets(line, MAX_BUFFER, file);
            length = strlen(line);

            if(length <= ADDRESS_LENGTH_OCT) // expected format
            {
                fprintf(stdout, "DEBUG: line: %s\n", line);
                if('1' == line[0])
                {
                    bAddy = true;
                    bPairFound = false;
                }
                else if('0' == line[0])
                {
                    bAddy = false;
                    bPairFound = false;
                }
                else
                {
                    Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
                    bPairFound = false;
                }

                //buffer.push_back(line);
                if(length == maxOctLeng)
                {
                    sInput[0] = line[1];
                    sInput[1] = line[2];
                }
                else if(length == 3)
                {
                    sInput[0] = '0';
                    sInput[1] = line[2];
                }
                else if(length == 2)
                {
                    Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
                }

                fgets(line, MAX_BUFFER, file);
                length = strlen(line);
                if(length <= ADDRESS_LENGTH_OCT) // expected format
                {
                    fprintf(stdout, "DEBUG: line: %s\n", line);
                    //buffer.push_back(line);
                    sInput[2] = line[1];
                    sInput[3] = line[2];

                    if(length == maxOctLeng)
                    {
                        sInput[2] = line[1];
                        sInput[3] = line[2];
                    }
                    else if(length == 3)
                    {
                        sInput[2] = '0';
                        sInput[3] = line[2];
                    }
                    else if(length == 2)
                    {
                        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
                    }
                    
                    bPairFound = true;
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

            if(bPairFound) // Ignore blank lines
            {
                sInput[ADDRESS_LENGTH_OCT] = '\0';
                fprintf(stdout, "DEBUG: addy: %s\n", sInput);
                rInput.setReg(sInput);
                if(m_memory->checkValidAddy(&rInput))
                {
                    //debug
                    fprintf(stdout, "DEBUG: addy: %s\n", rInput.getString());

                    if(bAddy) //it's an address
                    {
                        RegisterFile.rPC->setReg(&rInput);
                    }
                    else //it's data
                    {
                        instructionFetch(&rInput);
                        instructionDecode();
                        instructionExecute();
                    }
                }
                else
                {
                    Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
                }
                
            }
        }
        fclose(file);
    }

    data = 0;
    rInput.setReg(data);
    data = 100;
    rData.setReg(data);
    m_memory->store(&rInput, &rData);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void ControlUnit::printMemoryHistory(char* filename)
{
    if(m_memory && filename)
    {
        m_memory->dump_memory(filename);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
InstFormat::InstFormat()
{
    m_rInstruction = new BitReg(REG_12BIT);
    m_rCurrAddy = new BitReg(REG_12BIT);
    m_rOpcode = new BitReg(REG_3BIT);
    m_rOffset = new BitReg(REG_7BIT);
    m_rOffext = new BitReg(REG_9BIT);
    m_opTable = new OpTableHandle;
    reset();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
InstFormat::~InstFormat()
{
    if(m_rInstruction)
    {
        delete m_rInstruction;
        m_rInstruction = NULL;
    }
    if(m_rCurrAddy)
    {
        delete m_rCurrAddy;
        m_rCurrAddy = NULL;
    }
    if(m_rOpcode)
    {
        delete m_rOpcode;
        m_rOpcode = NULL;
    }
    if(m_rOffset)
    {
        delete m_rOffset;
        m_rOffset = NULL;
    }
    if(m_rOffext)
    {
        delete m_rOffext;
        m_rOffext = NULL;
    }
    if(m_opTable)
    {
        delete m_opTable;
        m_opTable = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::reset()
{
    m_bMRI = false;
    m_bOperate = false;
    m_bTestIO = false;
    m_bIndirect = 0;
    m_bZeroPage = 0;
    m_iMicroCode = 0;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::setOpCode()
{
    BitReg* opcode = NULL;
    int opnum = 0;
    opcode = m_rInstruction->sliceBits(OPCODE_MIN_INDEX, OPCODE_LENGTH);
    if(opcode)
    {
        m_rOpcode->setReg(opcode);
        RegisterFile.rIR->setReg(opcode);
        opnum = opcode->getNumber();
        //debug
        fprintf(stdout, "DEBUG: opcode: %s, %s\n", opcode->getString(), m_opTable->getMnemonic(opnum));
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::setOffset()
{
    BitReg* offset = NULL;

    if(m_bMRI)
    {
        offset = m_rInstruction->sliceBits(OFFSET_MIN_INDEX, OFFSET_LENGTH);
        if(offset)
        { 
            m_rOffset->setReg(offset);
            //debug
            fprintf(stdout, "DEBUG: offset: %s  %s\n", m_rOffext->getString(), getInstType());
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }
    else if(m_bOperate)
    {
        offset = m_rInstruction->sliceBits(OFFSET_EXT_MIN_INDEX, OFFSET_EXT_LENGTH);
        if(offset)
        {
            m_rOffext->setReg(offset);
            //debug
            fprintf(stdout, "DEBUG: offset: %s  %s\n", m_rOffext->getString(), getInstType());
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }
    else if(m_bTestIO)
    {
        //debug
        fprintf(stdout, "DEBUG: offset: %s  %s\n", m_rOffext->getString(), getInstType());
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::setInstType()
{
    bool* btemp = NULL;
    btemp = RegisterFile.rIR->getBool();
    if(btemp)
    {
        BitReg temp(btemp);
        int opIndex = temp.getNumber();

        if((OPCODE_MAX >= opIndex) && (OPCODE_MIN <= opIndex))
        {
            if(OPCODE_MRI >= opIndex)
            {
                m_bMRI = true;
                m_bTestIO = false;
                m_bOperate = false;
            }
            else if(OPCODE_IO == opIndex)
            {
                m_bMRI = false;
                m_bTestIO = true;
                m_bOperate = false;
            }
            else if(OPCODE_OPP == opIndex)
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
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    if(btemp)
    {
        delete[] btemp;
        btemp = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::loadInstruction(BitReg* inst)
{
    bool* bInst = NULL;
    if(inst)
    {
        int length = inst->getLength();  
        bInst = inst->getBool();

        reset();

        if(REG_12BIT == length)
        {
            m_rInstruction->setReg(inst);
            setOpCode();
            setInstType();
            setOffset();

            if(m_bMRI)
            {
                m_bIndirect = bInst[INST_INDIRECT_BIT];
                m_bZeroPage = bInst[INST_MEMPAGE_BIT];
                //debug
                fprintf(stdout, "DEBUG: load instruction: %s\n", getInstType());
            }
            else if(m_bOperate)
            {
                m_iMicroCode = inst->getNumber();
                //debug
                fprintf(stdout, "DEBUG: load instruction: %s\n", getInstType());
            }
            else if(m_bTestIO)
            {
                //debug
                fprintf(stdout, "DEBUG: load instruction: %s\n", getInstType());
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
    }

    if(bInst)
    {
        delete[] bInst;
        bInst = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns the type of format in a string 
char* InstFormat::getInstType()
{
    char* temp = NULL;
    char* stemp1 = INST_FORM_MRI;
    char* stemp2 = INST_FORM_OP;
    char* stemp3 = INST_FORM_IO;

    if(m_bMRI)
    {
        temp = new char[strlen(stemp1)+1];
        strcpy(temp, stemp1);
    }
    else if(m_bOperate)
    {
        temp = new char[strlen(stemp2)+1];
        strcpy(temp, stemp2);
    }
    else if(m_bTestIO)
    {   
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//MRI format, 1 bit Indirect bit, 0 = none, 1 = indirection
bool InstFormat::getMRIindirect()
{
    return m_bIndirect;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//MRI format, 1 bit Memory page bit, 0 = zero page, 1 = current page
bool InstFormat::getMRIMempage()
{
    return m_bZeroPage;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool InstFormat::isInstMRI()
{
    return m_bMRI;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool InstFormat::isInstOperate()
{
    return m_bOperate;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool InstFormat::isInstTestIO()
{
    return m_bTestIO;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//MRI format, 7 bit offset
BitReg* InstFormat::getMRIoffset()
{
    bool* btemp = NULL;
    BitReg* temp = NULL;
    btemp = m_rOffset->getBool();
    if(btemp)
    {
        temp = new BitReg(btemp);
        delete [] btemp;
        btemp = NULL;
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//Operate format, 9 bit extended opcode
BitReg* InstFormat::getOPextended()
{
    bool* btemp = NULL;
    BitReg* temp = NULL;
    btemp = m_rOffext->getBool();
    if(btemp)
    {
        temp = new BitReg(btemp);
        delete[] btemp;
        btemp = NULL;
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int InstFormat::getMicroCode()
{
    return m_iMicroCode;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int InstFormat::getOpcode()
{
    return m_rOpcode->getNumber();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
BitReg* InstFormat::getInstruction()
{
    return m_rInstruction->getReg();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
BitReg* InstFormat::getAddress()
{
    return m_rCurrAddy->getReg();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::setAddress(BitReg* addy)
{
    m_rCurrAddy->setReg(addy);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
EffectiveAddress::EffectiveAddress()
{
    m_rOffset = new BitReg(REG_7BIT);
    m_rZeroPage = new BitReg(REG_5BIT);
    m_rCurrPage = new BitReg(REG_5BIT);
    m_bIndirect = 0;
    m_bMemory = 0;
    m_pMemory = NULL;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
EffectiveAddress::~EffectiveAddress()
{

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void EffectiveAddress::setPCOffset()
{
    BitReg* offset = NULL;
    bool* temp = NULL;
    offset = RegisterFile.rPC->sliceBits(OFFSET_PC_MIN_INDEX, OFFSET_PC_LENGTH);
    if(offset)
    {
        temp = offset->getBool();
        if(temp)
        {
            m_rCurrPage->setReg(temp);
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
    if(offset)
    {
        delete offset;
        offset = NULL;
    }
    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//if the memory page bit is 0, calculate 00000 + offset, range is 0000o-0177o
BitReg* EffectiveAddress::effAdzeroPage()
{
    const int words = 2;
    BitReg temps[2];
    BitReg* temp = NULL;
    bool* ztemp = NULL;
    bool* otemp = NULL;

    ztemp = m_rZeroPage->getBool();
    otemp = m_rOffset->getBool();
    if(ztemp && otemp)
    {
        temps[0].setReg(ztemp);
        temps[1].setReg(otemp);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    temp = temps->ccatRegs(temps, words);

    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(otemp)
    {
        delete[] otemp;
        otemp = NULL;
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//if the memory page bit is 1, calculate instruction page + offset, range instruction page
BitReg* EffectiveAddress::effAdcurrentPage()
{
    const int words = 2;
    BitReg temps[2];
    BitReg* temp = NULL;
    bool* ztemp = NULL;
    bool* otemp = NULL;

    ztemp = m_rCurrPage->getBool();
    otemp = m_rOffset->getBool();
    if(ztemp && otemp)
    {
        temps[0].setReg(ztemp);
        temps[1].setReg(otemp);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    temp = temps->ccatRegs(temps, words);

    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(otemp)
    {
        delete[] otemp;
        otemp = NULL;
    }

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//if the indirect bit is 1, calculate  c(address), range is any address but costs 2 mem reads
BitReg* EffectiveAddress::effAdindirectAddressZero()
{
    bool* ztemp = NULL;
    bool* otemp = NULL;
    BitReg* addy = NULL;
    BitReg* offset = NULL; 
    BitReg* temp = NULL;

    ztemp = effAdzeroPage()->getBool();
    if(ztemp)
    {
        temp = new BitReg(ztemp);
        if(temp)
        {
            otemp = readIndirect(temp)->getBool();
            if(otemp)
            {
                offset = new BitReg(otemp);
                m_rOffset->setReg(offset); //save the new offset
                addy = effAdzeroPage(); //recalcuate zero page with new offset
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_CONTROL);
            }
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(otemp)
    {
        delete[] otemp;
        otemp = NULL;
    }
    if(temp)
    {
        delete temp;
        temp = NULL;
    }
    if(offset)
    {
        delete offset;
        offset = NULL;
    }

    return addy;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//if the indirect bit is 1, calculate  c(address), range is any address but costs 2 mem reads
BitReg* EffectiveAddress::effAdindirectAddressCurr()
{
    bool* ztemp = NULL;
    bool* otemp = NULL;
    BitReg* addy = NULL;
    BitReg* offset = NULL;
    BitReg* temp = NULL;

    ztemp = effAdcurrentPage()->getBool();
    if(ztemp)
    {
        temp = new BitReg(ztemp);
        if(temp)
        {
            otemp = readIndirect(temp)->getBool();
            if(otemp)
            {
                offset = new BitReg(otemp);
                m_rOffset->setReg(offset); //save the new offset
                addy = effAdcurrentPage(); //recalcuate zero page with new offset
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_CONTROL);
            }
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(otemp)
    {
        delete[] otemp;
        otemp = NULL;
    }
    if(temp)
    {
        delete temp;
        temp = NULL;
    }
    if(offset)
    {
        delete offset;
        offset = NULL;
    }

    return addy;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//calculate c(0010o - 0017o) + 1 effective address
BitReg* EffectiveAddress::effAdautoIndexing()
{
    //TODO
    BitReg* temp = NULL;
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void EffectiveAddress::loadOffset(BitReg* reg)
{
    BitReg* offset = NULL;
    if(reg)
    {
        offset = reg->sliceBits(OFFSET_MIN_INDEX, OFFSET_LENGTH);
        if(offset)
        {
            m_rOffset->setReg(offset->getBool());
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }

    if(offset)
    {
        delete offset;
        offset = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
BitReg* EffectiveAddress::readIndirect(BitReg* reg)
{
    BitReg* regMB = NULL;
    BitReg* offset = NULL;

    if(m_pMemory && reg)
    {
        m_pMemory->load(reg);
        offset = RegisterFile.rMB->sliceBits(OFFSET_MIN_INDEX, OFFSET_LENGTH);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    return offset;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void EffectiveAddress::setMemory(Memory* mem)
{
    m_pMemory = mem;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//inputs an MRI instruction and a pointer to the memory
//returns 12 bit address of a memory location
BitReg* EffectiveAddress::getAddress(BitReg* reg)
{
    BitReg* tempReg = NULL;
    bool* temp = NULL; 

    if(reg)
    {
        int length = reg->getLength();
        temp = reg->getBool();

        if((REG_12BIT == length) && temp)
        {
            m_bIndirect = temp[INST_INDIRECT_BIT];
            m_bMemory = temp[INST_MEMPAGE_BIT];

            loadOffset(reg);

            //zero page offset
            if(!m_bMemory && !m_bIndirect)
            {
                tempReg = effAdzeroPage();
            }
            //zero page indirect
            else if(!m_bMemory && m_bIndirect)
            {
                tempReg = effAdindirectAddressZero();
            }
            //current page, no indirect
            else if(m_bMemory && !m_bIndirect)
            {
                tempReg = effAdcurrentPage();
            }
            //current page and indirect
            else if(m_bMemory && m_bIndirect)
            {
                tempReg = effAdindirectAddressCurr();
            }
            //auto indexing?
            else
            {
                //TODO
            }
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONTROL);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }

    return tempReg;
}

