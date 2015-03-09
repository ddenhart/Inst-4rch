/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    Accumulator.cpp
Date:			    03/02/2015
Description:	 This file contains the class Accumulator
================================================================================== */

#include <cstring>
#include <cstdio>
#include "Common.h"
#include "BitReg.h"
#include "Accumulator.h"

//External objects
//================================================================================== 
extern ErrorTable Error;
extern RegTable RegisterFile;
extern bool silent;


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Accumulator::Accumulator()
{
    m_bZero = false;
    m_bNegative = false;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
Accumulator::~Accumulator()
{
    m_bZero = false;
    m_bNegative = false;
}



//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
BitReg* Accumulator::getAC()
{
    BitReg* temp = NULL;

    temp = RegisterFile.rAC->getReg();
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::setAC(BitReg* rac)
{
    if(rac)
    {
        RegisterFile.rAC->setReg(rac);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }
}



//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
BitReg* Accumulator::getLB()
{
    BitReg* temp = NULL;

    temp = RegisterFile.rLB->getReg();
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::setLB(BitReg* rlb)
{
    if(rlb)
    {
        RegisterFile.rLB->setReg(rlb);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::sumReg(BitReg* rInReg)
{
    bool overflow = false;
    bool* temp = NULL;
    bool sum[REG_12BIT];
    BitReg* rac = NULL;
    bool* areg = NULL;
    bool* tempOfs = NULL;
    int size = 0;
    int offset = 0;
    int index = 0;
    
    rac = getAC();

    if(rInReg && rac)
    {
        areg = rac->getBool();
        temp = rInReg->getBool();
        size = rInReg->getLength();

        if(temp && areg)
        {
            if(REG_12BIT > size)
            {
                tempOfs = temp;
                temp = new bool[REG_12BIT];
                offset = REG_12BIT - size;

                for(int k = 0; k < offset; ++k)
                {
                    temp[k] = 0;
                }

                for(int j = offset; j < size; ++j)
                {
                    temp[j] = tempOfs[index];
                    ++index;
                }
            }

            index = REG_12BIT-1;
            for(int i = index; i>=0; --i)
            {
                if(temp[i] && areg[i])//if 11 then carry
                {
                    if(overflow)//previous carry from before so 1+1+1
                    {
                        sum[i] = 1;
                    }
                    else //no carry so 1+1+0
                    {
                        sum[i] = 0;
                    }
                    overflow = true;
                }
                else if(temp[i] || areg[i])//otherwise 01,10
                {
                    if(overflow)//previous carry from before so 0+1+1
                    {
                        sum[i] = 0;
                        overflow = true;
                    }
                    else //no carry so 0+1+0
                    {
                        sum[i] = 1;
                        overflow = false;
                    }
                }
                else //else 00
                {
                    if(overflow)//previous carry from before so 0+0+1
                    {
                        sum[i] = 1;
                    }
                    else //no carry so 0+0+0
                    {
                        sum[i] = 0;
                    }
                    overflow = false;
                }
            }
            
            setCarry(&overflow);
            BitReg rsum(sum);
            setAC(&rsum);

#ifdef DEBUG_ALU
            fprintf(stdout, "DEBUG ALU: sum is %s\n", rsum.getString());
#endif
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(rac)
    {
        delete rac;
        rac = NULL;
    }
    if(areg)
    {
        delete[] areg;
        areg = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::setCarry(bool* btemp)
{
    BitReg temp(btemp);
    setLB(&temp);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool Accumulator::isNegative()
{
    bool temp = false;
    BitReg* rac = NULL;

    rac = getAC();
    bool* reg = rac->getBool();

    if(reg)
    {
        if(1 == reg[0])
        {
            temp = true;
        }
    }

    if(rac)
    {
        delete rac;
        rac = NULL;
    }

    return temp;
}


//================================================================================== 
//Name:
//Description: checks if
//Inputs:
//Outputs:
//Return:
//================================================================================== 
bool Accumulator::isZero()
{
    bool temp = false;
    BitReg* rac = NULL;

    rac = getAC();
    unsigned int num = rac->getNumber();

    if(0 == num)
    {
        temp = true;
    }

    if(rac)
    {
        delete rac;
        rac = NULL;
    }

    return temp;
}


//================================================================================== 
//Name:
//Description: complement each bit in the accumulator
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::complement()
{
    BitReg* rac = NULL;
    BitReg* rlb = NULL;
    bool* carry = NULL;

    rac = getAC();
    rlb = getLB();

    if(rac && rlb)
    {
        carry = rlb->getBool();
        rac->complement();
        setAC(rac);
        carry[0] = !carry[0];
        setCarry(carry);

 #ifdef DEBUG_ALU
        fprintf(stdout, "DEBUG ALU: complement is %s and link is %d\n",
                    rac->getString(), (!carry));
#endif
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(rac)
    {
        delete rac;
        rac = NULL;
    }
    if(rlb)
    {
        delete rlb;
        rlb = NULL;
    }
    if(carry)
    {
        delete carry;
        rac = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::increment(BitReg* rInReg)
{
    bool carry = false;
    BitReg rOne(1, REG_12BIT);  //add one
    setCarry(&carry); //clear the carry before setting the accumulator to 1
    setAC(&rOne); //set the accumulator to 1
    sumReg(rInReg); //add the input register 
#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG ALU: increment is %s\n",
            rInReg->getString());
#endif
}


//================================================================================== 
//Name:
//Description: 2's complement negate
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::negate()
{
    BitReg* rInReg = NULL;
    bool carry = true; //set true to simulate adding one
    bool* temp = NULL;
    int size = 0;

    rInReg = getAC();

    if(rInReg)
    {
        rInReg->complement();
        temp = rInReg->getBool();
        size = rInReg->getLength();

        if(temp)
        {
            for(int i = (size-1); i >= 0; --i)
            {
                if(temp[i] && carry) //case 11
                {
                    temp[i] = 0;
                    carry = true;
                }
                else if((temp[i] && !carry) || (!temp[i] && carry)) //case 01 or 10
                {
                    temp[i] = 1;
                    carry = false;
                }
                else //case 00
                {
                    temp[i] = 0;
                    carry = false;
                }
            }

            BitReg reg(temp);
            setCarry(&carry);
            setAC(&reg);
#ifdef DEBUG_ALU
            fprintf(stdout, "DEBUG ALU: negate is %s and carry is %d\n",
                    reg.getString(), carry);
#endif
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_BITREG);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    if(rInReg)
    {
        delete rInReg;
        rInReg = NULL;
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
void Accumulator::clear()
{
    unsigned int num = 0;
    bool carry = false;
    BitReg reg(num,REG_12BIT);

    setCarry(&carry);
    setAC(&reg);
#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG ALU: clear is %s\n",
            reg.getString());
#endif
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::rotateRight()
{
    BitReg* rac = NULL;
    BitReg* rlb = NULL;
    BitReg* shiftedrac = NULL;
    bool* temp = NULL;
    bool* ztemp = NULL;
    bool* brlb = NULL;
    int size = 0;

    rac = getAC();
    rlb = getLB();

    if(rac && rlb)
    {
        temp = rac->getBool();
        size = rac->getLength();
        shiftRight(1);
        shiftedrac = getAC();
        brlb = rlb->getBool();

        if(temp && shiftedrac && brlb)
        {
            ztemp = shiftedrac->getBool();
            if(ztemp)
            {
                ztemp[0] = brlb[0];
                brlb[0] = temp[(size -1)];

                setCarry(brlb);
                BitReg rtemp(ztemp);
                setAC(&rtemp);

#ifdef DEBUG_ALU
                fprintf(stdout, "DEBUG ALU: rotate right is %s and carry is %d\n",
                        rtemp.getString(), brlb[0]);
#endif
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_ALU);
            }
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(rac)
    {
        delete rac;
        rac = NULL;
    }
    if(rlb)
    {
        delete rlb;
        rlb = NULL;
    }
    if(shiftedrac)
    {
        delete shiftedrac;
        shiftedrac = NULL;
    }
    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(brlb)
    {
        delete[] brlb;
        brlb = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::rotateLeft()
{
    BitReg* rac = NULL;
    BitReg* rlb = NULL;
    BitReg* shiftedrac = NULL;
    bool* temp = NULL;
    bool* ztemp = NULL;
    bool* brlb = NULL;
    int size = 0;

    rac = getAC();
    rlb = getLB();

    if(rac && rlb)
    {
        temp = rac->getBool();
        size = rac->getLength();
        shiftLeft(1);
        shiftedrac = getAC();
        brlb = rlb->getBool();

        if(temp && shiftedrac && brlb)
        {
            ztemp = shiftedrac->getBool();
            if(ztemp)
            {
                ztemp[(size-1)] = brlb[0];
                brlb[0] = temp[0];

                setCarry(brlb);
                BitReg rtemp(ztemp);
                setAC(&rtemp);

#ifdef DEBUG_ALU
                fprintf(stdout, "DEBUG ALU: rotate left is %s and carry is %d\n",
                        rtemp.getString(), brlb[0]);
#endif
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_ALU);
            }
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(rac)
    {
        delete rac;
        rac = NULL;
    }
    if(rlb)
    {
        delete rlb;
        rlb = NULL;
    }
    if(shiftedrac)
    {
        delete shiftedrac;
        shiftedrac = NULL;
    }
    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(brlb)
    {
        delete[] brlb;
        brlb = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::shiftRight(int num)
{
    BitReg* reg = NULL;
    bool* temp = NULL;
    bool* ztemp = NULL;
    bool carry = false;
    int size = 0;
    int index = 0;
    bool bextend = 0;

    reg = getAC();

    if(reg)
    {
        temp = reg->getBool();
        size = reg->getLength();

        if(temp)
        {
            ztemp = new bool[size];
            bextend = temp[0];
            for(int i = 0; i < num; ++i)
            {
                if(i < size)
                {
                    ztemp[i] = bextend; //sign extension
                }
                else
                {
                    Error.printError(ERROR_OUT_OF_RANGE, FILE_ALU);
                    break;
                }
            }
            for(int j = num; j < size; ++j)
            {
                ztemp[j] = temp[index];
                ++index;
            }

            setCarry(&carry);
            BitReg rtemp(ztemp);
            setAC(&rtemp);

#ifdef DEBUG_ALU
            fprintf(stdout, "DEBUG ALU: shift right is %s\n",
                        rtemp.getString());
#endif      
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }

    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(reg)
    {
        delete reg;
        reg = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::shiftLeft(int num)
{
    BitReg* reg = NULL;
    bool* temp = NULL;
    bool* ztemp = NULL;
    bool carry = false;
    int size = 0;
    int index = 0;
    int offset = 0;

    reg = getAC();

    if(reg && (num > 0))
    {
        temp = reg->getBool();
        size = reg->getLength();

        if(temp)
        {
            ztemp = new bool[size];
            for(int i = num; i < size; ++i)
            {
                ztemp[index] = temp[i];
                ++index;
            }
            offset = index; //size - num
            for(int j = offset; j < size; ++j)
            {
                ztemp[j] = 0;
            }

            carry = temp[(num-1)];
            setCarry(&carry);
            BitReg rtemp(ztemp);
            setAC(&rtemp);

#ifdef DEBUG_ALU
            fprintf(stdout, "DEBUG ALU: shift left is %s\n",
                    rtemp.getString());
#endif
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }

    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(reg)
    {
        delete reg;
        reg = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::andReg(BitReg* rInReg)
{
    BitReg* reg = NULL;
    bool* temp = NULL;
    bool* atemp = NULL;
    bool* ztemp = NULL;
    bool resulttemp[REG_12BIT];
    bool carry = false;
    int size = 0;
    int index = 0;
    int offset = 0;

    reg = getAC();

    if(reg && rInReg)
    {
        atemp = reg->getBool();
        temp = rInReg->getBool();
        size = rInReg->getLength();

        if(temp && atemp)
        {
            if(REG_12BIT > size)
            {
                ztemp = temp;
                temp = new bool[REG_12BIT];
                offset = REG_12BIT - size;
                for(int j = 0; j < offset; ++j)
                {
                    temp[j] = 0; //sign extend
                }

                for(int k = offset; k < REG_12BIT; ++k)
                {
                    temp[k] = ztemp[index];
                    ++index;
                }
            }

            for(int i = 0; i < REG_12BIT; ++i)
            {
                resulttemp[i] = atemp[i] && temp[i];
            }

            setCarry(&carry);
            BitReg rtemp(resulttemp);
            setAC(&rtemp);

#ifdef DEBUG_ALU
            fprintf(stdout, "DEBUG ALU: and is %s\n",
                    rtemp.getString());
#endif
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }

    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(atemp)
    {
        delete[] atemp;
        atemp = NULL;
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(reg)
    {
        delete reg;
        reg = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::orReg(BitReg* rInReg)
{
    BitReg* reg = NULL;
    bool* temp = NULL;
    bool* atemp = NULL;
    bool* ztemp = NULL;
    bool resulttemp[REG_12BIT];
    bool carry = false;
    int size = 0;
    int index = 0;
    int offset = 0;

    reg = getAC();

    if(reg && rInReg)
    {
        atemp = reg->getBool();
        temp = rInReg->getBool();
        size = rInReg->getLength();

        if(temp && atemp)
        {
            if(REG_12BIT > size)
            {
                ztemp = temp;
                temp = new bool[REG_12BIT];
                offset = REG_12BIT - size;
                
                for(int j = 0; j < offset; ++j)
                {
                    temp[j] = 0; //sign extend
                }

                for(int k = offset; k < REG_12BIT; ++k)
                {
                    temp[k] = ztemp[index];
                    ++index;
                }
            }

            for(int i = 0; i < REG_12BIT; ++i)
            {
                resulttemp[i] = atemp[i] || temp[i];
                ++index;
            }

            setCarry(&carry);
            BitReg rtemp(resulttemp);
            setAC(&rtemp);

#ifdef DEBUG_ALU
            fprintf(stdout, "DEBUG ALU: or is %s\n",
                    rtemp.getString());
#endif
        }
        else
        {
            Error.printError(ERROR_NULL, FILE_ALU);
        }

    }
    else
    {
        Error.printError(ERROR_NULL, FILE_ALU);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
    if(ztemp)
    {
        delete[] ztemp;
        ztemp = NULL;
    }
    if(atemp)
    {
        delete[] atemp;
        atemp = NULL;
    }
    if(reg)
    {
        delete reg;
        reg = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::printAll()
{
    char num[] = { '7', '0', '0', '7','\0' };
    BitReg temp(num);

    printALU();
    increment(&temp);
    printALU();
    sumReg(&temp);
    printALU();
    andReg(&temp);
    printALU();
    orReg(&temp);
    printALU();
    shiftLeft(2);
    printALU();
    shiftRight(2);
    printALU();
    rotateLeft();
    printALU();
    rotateRight();
    printALU();
    negate();
    printALU();
    complement();
    printALU();
    clear();
    printALU();
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void Accumulator::printALU()
{
    BitReg* reg = NULL;
    BitReg* blink = NULL;
    bool* temp = NULL;
    int conv = 0;

    reg = getAC();
    blink = getLB();

    if(reg && blink)
    {
        temp = blink->getBool();
        if(temp[0])
        {
            conv = 1;
        }
        fprintf(stdout, "DEBUG ALU: %s %d\n", reg->getBinary(), conv);
    }

    if(reg)
    {
        delete reg;
        reg = NULL;
    }
    if(blink)
    {
        delete blink;
        blink = NULL;
    }
    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }
}



