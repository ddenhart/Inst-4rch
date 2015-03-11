/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
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
#include <iostream>
#include "Common.h"
#include "OctConv.h"
#include "BitReg.h"
#include "OpTable.h"
#include "Memory.h"
#include "ControlUnit.h"
#include "Accumulator.h"

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
    silent = false;
    m_alu = new Accumulator;
    m_memory = new Memory;
    m_eAddy.setMemory(m_memory);
    char addy[] = "0200";
    m_StartAddress = new BitReg(addy); //had to change due to persistant warning
	running = true;
    for(int i = 0; i < OP_TABLE_LENG; ++i)
    {
        stats.instUsage[i] = 0;
    }
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
    silent = false;
    if(m_StartAddress)
    {
        delete m_StartAddress;
        m_StartAddress = NULL;
    }
    if(m_memory)
    {
        delete m_memory; 
        m_memory = NULL;
    }
    if(m_alu)
    {
        delete m_alu;
        m_alu = NULL;
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
void ControlUnit::modeHex(char* filename)
{
    char line[MAX_BUFFER]; // String for getline
    int length = 0;
    bool bFirstLine = true;
    bool* bReg = NULL;
    char sInput[ADDRESS_LENGTH_HEX + 1];
    BitReg rInput(REG_12BIT); // Current address

    file = fopen(filename, "r");

    if(file)
    {
        while((!feof(file)) && (fgets(line, MAX_BUFFER, file)))//otherwise prints an extra line
        {
            length = strlen(line);

            if(length == (ADDRESS_LENGTH_HEX+1))
            {
                //it's data
                bReg = m_conv.convertToBinaryHex(line);

                if(bReg)
                {
                    rInput.setReg(bReg);
#ifdef DEBUG_CONTROL
                    fprintf(stdout, "DEBUG address: %s\n", rInput.getString());
#endif
                    BitReg* rpc = getPC();
                    m_memory->store(rpc, &rInput);
                    RegisterFile.incrementPC();
                    if(rpc)
                    {
                        delete rpc;
                        rpc = NULL;
                    }
                }
                else
                {
                    Error.printError(ERROR_NULL, FILE_CONTROL);
                }
            }
            else if(length == (ADDRESS_LENGTH_OCT+1))
            {
                if('@' == line[0]) //address
                {
                    memmove(line, line+1, strlen(line)); //remove @
                    bReg = m_conv.convertToBinaryHex(line);

                    if(bReg)
                    {
                        rInput.setReg(bReg);
#ifdef DEBUG_CONTROL
                        fprintf(stdout, "DEBUG address: %s\n", rInput.getString());
#endif
                        if(m_memory->checkValidAddy(&rInput))  //check for valid address
                        {
                            if(bFirstLine)
                            {
                                m_StartAddress->setReg(&rInput);
                            }

                            bFirstLine = false;
                            setPC(&rInput);
                        }
                        else
                        {
                            Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
                        }
                    }
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
        }

        fclose(file);
    }
}


//==================================================================================
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
void ControlUnit::modeBin(char* filename)
{
    bool bFirstLine = true;
    bool* bBuf0 = NULL;
    bool* bBuf1 = NULL;
    bool bReg[REG_12BIT];
    unsigned char* buffer = NULL;
    int size0 = 0;
    int size1 = 0;
    int conv = 0;
    std::vector<char> sBuf;
    std::vector<char>::iterator it;
    BitReg rparse0(REG_8BIT);
    BitReg rparse1(REG_8BIT);
    BitReg rInput; // Current address

    file = fopen(filename, "r+b");

    if(file)
    {
        //get the size of the file
        fseek(file, 0, SEEK_END);
        unsigned long fsize = ftell(file);
        buffer = new unsigned char[fsize+1];
        rewind(file);

        //read the file
        fread(buffer, 1, fsize, file);
        fclose(file);
        buffer[fsize] = '\0';

        //get rid of headers
        for(int i = 0; i < fsize; ++i)
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
                sBuf.push_back(buffer[i]);
            }
        }
        
        //parse the binary
        for(it = sBuf.begin(); it != sBuf.end(); ++it)
        {
            conv = *it;
            rparse0.setReg(conv); //set a byte
            bBuf0 = rparse0.getBool();
            size0 = rparse0.getLength();

            ++it;
            if(sBuf.end() == it)
            {
                fprintf(stderr, "%s\n", PRINT_BREAK);
                throw fprintf(stderr, "Error: Invalid binary file format...\n");
                fprintf(stderr, "%s\n", PRINT_BREAK);
                return;
            }
            else
            {
                conv = *it;
            }

            rparse1.setReg(conv); //set a byte
            bBuf1 = rparse1.getBool();
            size1 = rparse1.getLength();
            
            if(bBuf0 && bBuf1 && (REG_8BIT == size0) && (REG_8BIT == size1))
            {
                for(int i = 0; i < INPUT_OCTBYTE_MAX; ++i)
                {
                    bReg[i] = bBuf0[(i+2)];
                    bReg[(i+6)] = bBuf1[(i+2)];
                }

                rInput.setReg(bReg);
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG address: %s\n", rInput.getString());
#endif

                if(bBuf0[1]) //its an address
                {
                    if(m_memory->checkValidAddy(&rInput))  //check for valid address
                    {
                        if(bFirstLine)
                        {
                            m_StartAddress->setReg(&rInput);
                        }

                        bFirstLine = false;
                        setPC(&rInput);
                    }
                    else
                    {
                        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
                    }
                }
                else //its data
                {
                    BitReg* rpc = getPC();
                    m_memory->store(rpc, &rInput);
                    RegisterFile.incrementPC();
                    if(rpc)
                    {
                        delete rpc;
                        rpc = NULL;
                    }
                }

            }
            else
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
            }

            if(bBuf0)
            {
                delete[] bBuf0;
                bBuf0 = NULL;
            }
            if(bBuf1)
            {
                delete[] bBuf1;
                bBuf1 = NULL;
            }
        }
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONTROL);
    }

    if(buffer)
    {
        delete[] buffer;
        buffer = NULL;
    }

    if(bBuf0)
    {
        delete[] bBuf0;
        bBuf0 = NULL;
    }
    if(bBuf1)
    {
        delete[] bBuf1;
        bBuf1 = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
    void ControlUnit::modeOct(char* filename)
{
    char line[MAX_BUFFER]; // String for getline
    char sInput[ADDRESS_LENGTH_OCT + 1];
    int maxOctLeng = 4;
    bool bPairFound = false;
    bool bAddy = false;
    bool bFirstLine = true;
    BitReg rInput(REG_12BIT); // Current address 
    BitReg rData(REG_12BIT); // Current address 
    unsigned int data = 0;
    int length = 0;

    file = fopen(filename, "r");

    if(file)
    {
        while((!feof(file)) && (fgets(line, MAX_BUFFER, file)))//otherwise prints an extra line
        {
            length = strlen(line);
			//somehow our code conflict here... just commenting out till we can fix
			 /*int tempLen = length - 1;
			   if ((tempLen > 0) && (line[tempLen] == '\n'))
				    line[tempLen] = '\0';
			   length = strlen(line);*/
            if(length <= ADDRESS_LENGTH_OCT) // expected format
            {
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
			//somehow our code conflict here... just commenting out till we can fix
				    /*tempLen = length - 1;
				    if ((tempLen > 0) && (line[tempLen] == '\n'))
					    line[tempLen] = '\0';
				    length = strlen(line);*/
                if(length <= ADDRESS_LENGTH_OCT) // expected format
                {
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
                rInput.setReg(sInput);
                //debug
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG address: %s\n", rInput.getString());
#endif
                if(bAddy) //it's an address
                {
                    if(m_memory->checkValidAddy(&rInput))  //check for valid address
                    {
                        if(bFirstLine)
                        {
                            m_StartAddress->setReg(&rInput);
                        }

                        bFirstLine = false;
                        setPC(&rInput);
                    }
                }
                else //it's data
                {
                    BitReg* rpc = getPC();
                    m_memory->store(rpc, &rInput);
                    RegisterFile.incrementPC();
                    if(rpc)
                    {
                        delete rpc;
                        rpc = NULL;
                    }
                }
            }
            else
            {
                Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
            }
        }

        fclose(file);
    }
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
    BitReg ops(m_format.getOpcode());

    currInst = m_format.getInstruction();

    //up cycle count
    m_format.incrementCycles(&ops);

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
#ifdef DEBUG_CONTROL
		printf("cURRENT INST: %s\n", currInst->getString());
		printf("m_eaddy %s\n", m_eAddy.getAddress(currInst)->getString());
		m_format.setAddress(m_eAddy.getAddress(currInst));
#endif
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
        BitReg ops(m_format.getOpcode());
        //up cycle count
        m_format.incrementCyclesDefer(&ops);
        //RegisterFile.incrementPC();
#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG: Defer: extra cycle");
#endif
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
    BitReg* data = NULL;
	//Accumulator* accum = NULL;
	Opcode7List m_op7;

	int skipIncrement = false;

    unsigned int opcode = m_format.getOpcode();

    addy = m_format.getAddress();
#ifdef DEBUG_CONTROL
	printf("OPCODE %d\n\n\n", opcode);
#endif
    rpc = getPC();

    if(addy && rpc)
    {
        if(OPCODE_AND == opcode)
        {
            m_memory->load(addy);
            data = m_memory->readMB();
#ifdef DEBUG_CONTROL
            fprintf(stdout, "DEBUG Execute: %s\n", data->getString());
#endif
			m_alu->andReg(data);
        }
        else if(OPCODE_TAD == opcode)
        {
            m_memory->load(addy);
			m_alu->sumReg(m_memory->readMB());
        }
        else if(OPCODE_ISZ == opcode)
        {
            m_memory->load(addy);
			data = m_memory->readMB();
			data->setReg((data->getNumber2sComp() + 1));
			m_memory->store(addy, data);
			if (data->getNumber() == 0)
			{
				RegisterFile.incrementPC();
			}
        }
        else if(OPCODE_DCA == opcode)
        {
            //store result of accumulator
			m_memory->store(addy, m_alu->getAC());
			m_alu->clear();
        }
        else if(OPCODE_JMS == opcode)
        {
            m_memory->load(addy);
			data = m_memory->readMB();
			data->setReg(rpc->getNumber());
			m_memory->store(addy, data);
			m_memory->load(addy);
			data = m_memory->readMB();
			rpc->setReg((addy->getNumber()));
			setPC(rpc);
        }
        else if(OPCODE_JMP == opcode)
        {
            m_memory->load(addy);
			rpc->setReg(addy->getNumber());
			setPC(rpc);
			skipIncrement = true;
        }
        else if(OPCODE_IO == opcode)
        {
           //noop
            if(!silent)
            {
                fprintf(stdout, "Warning: NOP\n");
            }
        }
		else if (OPCODE_OPP == opcode)
		{
			BitReg* inst = NULL;
			inst = m_format.getInstruction();
			if(inst)
            {
                m_format.incrementMicros(inst); //update statistics
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_CONTROL);
            }
			m_memory->load(addy);
			data = m_memory->readMB();
			printf("Memory read: %s\n", data->getString());
			printf("Address: %s\n", addy->getString());
			for (int i = 0; i <= addy->getLength() - 1; ++i)
				printf("Addy: %c\n", addy->getBinary()[i]);
			if (addy->getBinary()[3] == '0')  //Group 1
			{
				BitReg rOne("0", REG_12BIT);
				for (int i = 4; i <= addy->getLength() - 1; ++i)
				{
					printf("Addy: %c\n", addy->getBinary()[i]);
					if (addy->getBinary()[i] == '1')
					{
						if (i == 4)
						{
							m_alu->clearAC();
						}
						else if (i == 5)
						{
							m_alu->clearLC();
						}
						else if (i == 6)
						{
							m_alu->complementAC();
						}
						else if (i == 7)
						{
							m_alu->complementLC();
							if (addy->getBinary()[11] == '1')
								m_alu->increment(&rOne);
						}
						else if (i == 8)
						{
							m_alu->rotateRight();
							if (addy->getBinary()[10] == '1')
								m_alu->rotateRight();
						}
						else if (i == 9)
						{
							m_alu->rotateLeft();
							if (addy->getBinary()[10] == '1')
								m_alu->rotateLeft();
						}
					}
				}
			}
			else
			{
				if (addy->getBinary()[8] == '0')
				{
					if ((addy->getBinary()[5] == '1') || (addy->getBinary()[6] == '1') || (addy->getBinary()[7] == '1'))
					{
						if (addy->getBinary()[5] == '1')
						{
							bool trial = m_alu->isNegative();
							if (trial)
								RegisterFile.incrementPC();
						}
						if (addy->getBinary()[6] == '1')
						{
							bool trial = m_alu->isZero();
							if (trial)
								RegisterFile.incrementPC();
						}
						if (addy->getBinary()[7] == '1')
						{
							BitReg* rlb = NULL;
							rlb = m_alu->getLB();
							if (rlb->getNumber2sComp() != 0)
								RegisterFile.incrementPC();
						}

					}
				}
				else
				{
					int skip = false;
					/*if ((addy->getBinary()[5] == '1' && addy->getBinary()[6] == '1') || (addy->getBinary()[5] == '1' && addy->getBinary()[7] == '1') || (addy->getBinary()[6] == '1' && addy->getBinary()[7] == '1'))
					{
						RegisterFile.incrementPC();
					}*/
					if (addy->getBinary()[5] == '1')
					{
						bool trial = m_alu->isNegative();
						if (!trial)
						{
							//RegisterFile.incrementPC();
							skip = true;
						}
					}
					if (addy->getBinary()[6] == '1')
					{
						skip = false;
						bool trial = m_alu->isZero();
						if (!trial)
						{
							//RegisterFile.incrementPC();
							skip = true;
						}
					}
					if (addy->getBinary()[7] == '1')
					{
						skip = false;
						BitReg* rlb = NULL;
						rlb = m_alu->getLB();
						if (rlb->getNumber2sComp() == 0)
						{
							//RegisterFile.incrementPC();
							skip = true;
						}
					}
					if (skip)
						RegisterFile.incrementPC();
					if (addy->getBinary()[4] == '0' && addy->getBinary()[5] == '0' && addy->getBinary()[6] == '0' && addy->getBinary()[7] == '0' && addy->getBinary()[8] == '1' && addy->getBinary()[9] == '0' && addy->getBinary()[10] == '0' && addy->getBinary()[11] == '0')
					{
						RegisterFile.incrementPC();
					}
					if (addy->getBinary()[4] == '1')
						m_alu->clearAC();
					if (addy->getBinary()[4] == '1')
						running = false;
				}
			}
        }
        else
        {
            Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONTROL);
        }
		if (!skipIncrement)
		      RegisterFile.incrementPC();
#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG Execute: %s  %s  PC: %s\n",
                    m_format.getInstType(), addy->getString(), rpc->getString());
#endif
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
void ControlUnit::loadFile(char* filename, int mode)
{
    BitReg rInput(REG_12BIT); // Current address
    BitReg rData(REG_12BIT); // Current address
    unsigned int data = 0;
    int length = 0;
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

   setPC(m_StartAddress);
   BitReg* rpc = getPC();
   m_memory->load(rpc);
   while(HALT_CODE != m_memory->readMB()->getNumber())
   {
		  instructionFetch(m_memory->readMB());
		  instructionDecode();
		  instructionExecute();
		  rpc = getPC();
        bMemValid = m_memory->pcMemoryValid();
        if(!bMemValid)
        {
            //throw error if memory access is out of bounds
            fprintf(stderr, "%s\n", PRINT_BREAK);
            throw fprintf(stderr, "Error: Exceeded memory space...\n");
            fprintf(stderr, "%s\n", PRINT_BREAK);
            return;
        }
        m_memory->load(rpc);
        if (rpc)
        {
            delete rpc;
            rpc = NULL;
        }
   }

   BitReg mem(m_memory->readMB()->getNumber());
   if(HALT_CODE == mem.getNumber())
   {
       m_format.incrementMicros(&mem);
   }

#ifdef DEBUG_CONTROL
   char stemp[5] = {'3', '7', '7', '7', '\0'};
   BitReg rbtemp(stemp);
   fprintf(stdout, "DEBUG control: %s, %d \n", rbtemp.getString(), rbtemp.getNumber2sComp());
   m_alu->printAll();
#endif

   m_format.printStats();
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
        m_memory->writeMemoryAccesses(filename);
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
    unsigned int opnum = 0;
    opcode = m_rInstruction->sliceBits(OPCODE_MIN_INDEX, OPCODE_LENGTH);
    if(opcode)
    {
        m_rOpcode->setReg(opcode);
        RegisterFile.rIR->setReg(opcode);
        opnum = opcode->getNumber();
        //debug
#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG opcode: %s, %s\n", opcode->getString(), m_opTable->getMnemonic(opnum));
#endif
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
#ifdef DEBUG_CONTROL
            fprintf(stdout, "DEBUG offset: %s  %s\n", m_rOffext->getString(), getInstType());
#endif
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
#ifdef DEBUG_CONTROL
            fprintf(stdout, "DEBUG offset: %s  %s\n", m_rOffext->getString(), getInstType());
#endif
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_CONTROL);
        }
    }
    else if(m_bTestIO)
    {
        //debug
#ifdef DEBUG_CONTROL
        fprintf(stdout, "DEBUG offset: %s  %s\n", m_rOffext->getString(), getInstType());
#endif
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
        unsigned int opIndex = temp.getNumber();

        if(OPCODE_MAX >= opIndex)// && (OPCODE_MIN <= opIndex)) <- always true
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
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG load instruction: %s\n", getInstType());
#endif
            }
            else if(m_bOperate)
            {
                m_iMicroCode = inst->getNumber();
                //debug
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG load instruction: %s\n", getInstType());
#endif
            }
            else if(m_bTestIO)
            {
                //debug
#ifdef DEBUG_CONTROL
                fprintf(stdout, "DEBUG load instruction: %s\n", getInstType());
#endif
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
unsigned int InstFormat::getMicroCode()
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
unsigned int InstFormat::getOpcode()
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
void InstFormat::incrementCycles(BitReg* ops)
{
    unsigned int index = 0;
    
    if(ops)
    {
        index = ops->getNumber();

        if(index < OP_TABLE_LENG)
        {
            m_opTable->incrementOpsCycle(ops->getNumber());
        }
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
void InstFormat::incrementCyclesDefer(BitReg* ops)
{
    unsigned int index = 0;

    if(ops)
    {
        index = ops->getNumber();

        if(index < OP_TABLE_LENG)
        {
            m_opTable->incrementOneCycle(index);
        }
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
void InstFormat::incrementMicros(BitReg* ops)
{

    unsigned int index = 0;

    if(ops)
    {
        index = ops->getNumber();
        index = convertToOctal(index);

        if(index >= 7000)
        {
            m_opTable->updateMicros(index);
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
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void InstFormat::printStats()
{
    m_opTable->printUsageAndCycles();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
unsigned int InstFormat::convertToOctal(unsigned int num)
{
    std::vector<unsigned int> buff;
    std::vector<unsigned int>::iterator it;
    int mod = 0;
    int size = 0;
    int factor = 1;
    int res = 0;


    while(num > 7)
    {
        mod = num%8;
        buff.push_back(mod);
        num = num/8;
    }
    mod = num%8;
    buff.push_back(mod);
    size = buff.size();
    it = buff.begin();

    for(int i = 0; i < size; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            factor *= 10;
        }

        res += (*it*factor);
        ++it;
        factor = 1;
    }

    return res;

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

	setPCOffset();
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
//Name: getAddress
//Description:inputs an MRI instruction and a pointer to the memory
//Inputs: a bitreg pointer of an address
//Outputs: none
//Return: 12 bit address of a memory location
//================================================================================== 
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

