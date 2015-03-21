/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke

==================================================================================
File:			    alu.cpp
Date:			    03/02/2015
Description:	 This file contains the class alu
================================================================================== */

#include <cstring>
#include <cstdio>
#include "Common.h"
#include "alu.h"

//External objects
//================================================================================== 
extern ErrorTable Error;
extern FILE* debugout;


//================================================================================== 
//Description: constructor
//================================================================================== 
alu::alu()
{
    m_bZero = false;
    m_bNegative = false;
    rLB = 0;
    rAC = 0;
}


//================================================================================== 
//Description: deconstructor
//================================================================================== 
alu::~alu()
{
    m_bZero = false;
    m_bNegative = false;
}



//================================================================================== 
//Description: returns the value of rAC
//================================================================================== 
unsigned short alu::getAC()
{
    return rAC;
}


//================================================================================== 
//Description: sets AC to a register value
//================================================================================== 
void alu::setAC(unsigned short rac)
{
    rAC = rac;
}



//================================================================================== 
//Description: returns the value of rLB
//================================================================================== 
bool alu::getLB()
{
    return rLB;
}


//================================================================================== 
//Description: sets the rLB to a value
//================================================================================== 
void alu::setLB(unsigned short rlb)
{
    rLB = rlb &= BIT0_MASK;
    //rLB = rlb;
}


//================================================================================== 
//Description: adds the input register to the contents of rALU
//================================================================================== 
void alu::sumALU(unsigned short reg)
{
    unsigned short temp = 0;

    temp = rAC + reg;

    if(temp > MAX_OCT_ADDRESS)
    {
        rLB = 1;
        temp = MAX_OCT_ADDRESS - rAC;
    }
    else
    {
        rLB = 0;
    }

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: sum alu rAC %o = %o + %o, rLB = %o\n", temp, reg, rAC, rLB);
    fprintf(debugout, "DEBUG: sum alu rAC %o = %o + %o, rLB = %o\n", temp, reg, rAC, rLB);
#endif

    rAC = temp;
}


//================================================================================== 
//Description: returns true if rAC's MSB is 1
//================================================================================== 
bool alu::isNegative()
{
    unsigned short temp = 0;

    temp = rAC & BIT11_MASK;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: checking if rAC is negative: %o\n", temp);
    fprintf(debugout, "DEBUG: checking if rAC is negative: %o\n", temp);
#endif

    return temp;
}


//================================================================================== 
//Description: returns true of rAC is 0
//================================================================================== 
bool alu::isZero()
{
    bool temp = false;

    if(0 == rAC)
    {
        temp = true;
    }

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: checking if rAC is 0: %o\n", temp);
    fprintf(debugout, "DEBUG: checking if rAC is 0: %o\n", temp);
#endif

    return temp;
}

//================================================================================== 
//Description: complement each bit in the alu
//================================================================================== 
void alu::complementLC()
{
    unsigned short temp = rLB;

    if(temp == 1)
    {
        rLB = 0;
    }
    else if(temp == 0)
    {
        rLB = 1;
    }
    else
    {
        Error.printError(ERROR_UNEXPECTED_VALUE, FILE_ALU);
    }

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: complemented rLB from %o to %o\n", temp, rLB);
    fprintf(debugout, "DEBUG: complemented rLB from %o to %o\n", temp, rLB);
#endif
}

//================================================================================== 
//Description: complement each bit in the rAC
//================================================================================== 
void alu::complementALU()
{
    unsigned short temp = rAC;

    rAC = MAX_OCT_ADDRESS - rAC;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: complemented rAC from %o to %o\n", temp, rAC);
    fprintf(debugout, "DEBUG: complemented rAC from %o to %o\n", temp, rAC);
#endif
}

//================================================================================== 
//Description: increments the value of rAC by 1
//================================================================================== 
void alu::incrementALU()
{
    unsigned short temp = rAC;

    sumALU(1);

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: incrementing from %o to %o\n", temp, rAC);
    fprintf(debugout, "DEBUG: incrementing from %o to %o\n", temp, rAC);
#endif
}


//================================================================================== 
//Description: 2's complement negate
//================================================================================== 
void alu::negateALU()
{
    unsigned short temp = rAC;

    complementALU();
    incrementALU();

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: negated rAC from %o to %o\n", temp, rAC);
    fprintf(debugout, "DEBUG: negated rAC from %o to %o\n", temp, rAC);
#endif
}

//==================================================================================
//Description: sets rLB to 0
//================================================================================== 
void alu::clearLC()
{
    rLB = 0;
}

//================================================================================== 
//Description: sets rAC to 0
//================================================================================== 
void alu::clearAC()
{
    rAC = 0;
}

//================================================================================== 
//Description: clears bot rLB and rAC
//================================================================================== 
void alu::clear()
{
    clearAC();
    clearLC();

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: cleared rAC: %o and rLB: %o\n", rAC, rLB);
    fprintf(debugout, "DEBUG: cleared rAC: %o and rLB: %o\n", rAC, rLB);
#endif
}


//================================================================================== 
//Description:shifts rAC right by one and carries the LSB to the MSB
//================================================================================== 
void alu::rotateRight()
{
    unsigned short temp = 0;
    unsigned short carry = 0;
    unsigned short rlb = 0;

    temp = rAC;
    carry = rLB;
    rlb = carry << REG_12BIT;
    rLB = rAC & BIT11_MASK;
    //rLB = carry >> REG_12BIT;
    rAC = rAC >> 1;
    rAC += rlb;
    rAC &= REG_12BIT_MASK;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: rotate right from %o %o to %o %o\n",
    //        carry, temp, rLB, rAC);
    fprintf(debugout, "DEBUG: rotate right from %o %o to %o %o\n",
            carry, temp, rLB, rAC);
#endif
}


//================================================================================== 
//Description: shifts rAC left by one and carries the MSB to the LSB
//================================================================================== 
void alu::rotateLeft()
{
    unsigned short temp = 0;
    unsigned short carry = 0;

    temp = rAC;
    carry = rLB;
    rAC = rAC << 1;
    rAC += carry;
    rLB = rAC & BIT_OVERFLOW_MASK;
    rLB = rLB >> REG_12BIT;
    rAC &= REG_12BIT_MASK;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: rotate left from %o %o to %o %o\n",
    //        carry, temp, rLB, rAC);
    fprintf(debugout, "DEBUG: rotate left from %o %o to %o %o\n",
            carry, temp, rLB, rAC);
#endif
}


//================================================================================== 
//Description: shifts rAC right by num amount
//================================================================================== 
void alu::shiftRight(unsigned short num)
{
    unsigned short temp = rAC;
    unsigned short carry = rLB;
    unsigned short opshift = REG_12BIT-num;

    rLB = rLB << opshift;
    rAC = rAC >> num;
    rAC += rLB;
    rAC &= REG_12BIT_MASK;
    rLB = 0;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: shift right by %d from %o %o to %o %o\n",
    //        num, carry, temp, rLB, rAC);
    fprintf(debugout, "DEBUG: shift right by %d from %o %o to %o %o\n",
            num, carry, temp, rLB, rAC);
#endif
}


//================================================================================== 
//Description: shifts rAC left by num amount
//================================================================================== 
void alu::shiftLeft(unsigned short  num)
{
    unsigned short temp = rAC;
    unsigned short carry = rLB;
    unsigned short opshift = REG_12BIT-num;

    rLB = rAC >> opshift;
    rLB &= BIT11_MASK;
    rAC = rAC << num;
    rAC &= REG_12BIT_MASK;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: shift left by %d from %o %o to %o %o\n", 
    //        num, carry, temp, rLB, rAC);
    fprintf(debugout, "DEBUG: shift left by %d from %o %o to %o %o\n", 
            num, carry, temp, rLB, rAC);
#endif
}


//================================================================================== 
//Description: ands the input reg with rAC
//================================================================================== 
void alu::andALU(unsigned short reg)
{
    unsigned short temp = rAC;
    rAC &= reg;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: and AC with %o from %o to %o\n", reg, temp, rAC);
    fprintf(debugout, "DEBUG: and AC with %o from %o to %o\n", reg, temp, rAC);
#endif
}


//================================================================================== 
//Description: ors the input reg with rAC
//================================================================================== 
void alu::orALU(unsigned short reg)
{
    unsigned short temp = rAC;
    rAC |= reg;

#ifdef DEBUG_ALU
    //fprintf(stdout, "DEBUG: or AC with %o from %o to %o\n", reg, temp, rAC);
    fprintf(debugout, "DEBUG: or AC with %o from %o to %o\n", reg, temp, rAC);
#endif
}


//================================================================================== 
//Description: prints a test of all functions of the alu
//================================================================================== 
void alu::printAll()
{
    unsigned short temp = 07007;

    printALU();
    incrementALU();
    printALU();
    sumALU(temp);
    printALU();
    andALU(temp);
    printALU();
    orALU(temp);
    printALU();
    shiftLeft(2);
    printALU();
    shiftRight(2);
    printALU();
    rotateLeft();
    printALU();
    rotateRight();
    printALU();
    negateALU();
    printALU();
    clear();
    printALU();
}


//================================================================================== 
//Description: prints the value of the alu in decimal, octal and hexidecimal
//================================================================================== 
void alu::printALU()
{
        fprintf(stdout, "ALU: dec: %d  oct: %o  hex: %x\n", rAC, rAC, rAC);
}


