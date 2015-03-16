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
void alu::setLB(bool rlb)
{
    rLB = rlb;
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
    fprintf(stdout, "DEBUG: sum alu rAC %o = %o + %o, rLB = %o\n", temp, reg, rAC, rLB);
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
    fprintf(stdout, "DEBUG: checking if rAC is negative: %o\n", temp);
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
    fprintf(stdout, "DEBUG: checking if rAC is 0: %o\n", temp);
#endif

    return temp;
}

//================================================================================== 
//Description: complement each bit in the alu
//================================================================================== 
void alu::complementLC()
{
    bool temp = rLB;

    rLB = !rLB;

#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG: complemented rLB from %o to %o\n", temp, rLB);
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
        fprintf(stdout, "DEBUG: complemented rAC from %o to %o\n", temp, rAC);
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
    fprintf(stdout, "DEBUG: incrementing from %o to %o\n", temp, rAC);
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
    fprintf(stdout, "DEBUG: negated rAC from %o to %o\n", temp, rAC);
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
    fprintf(stdout, "DEBUG: cleared rAC: %o and rLB: %o\n", rAC, rLB);
#endif
}


//================================================================================== 
//Description:shifts rAC right by one and carries the LSB to the MSB
//================================================================================== 
void alu::rotateRight()
{
    unsigned short temp = 0;
    unsigned short carry = 0;

    temp = rAC;
    carry = rAC & BIT11_MASK;
    carry = carry << REG_12BIT;
    rAC = rAC >> 1;
    rAC &= REG_12BIT_MASK;
    rAC += carry;

#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG: rotate right from %o to %o with a carry %o\n",
            temp, rAC, carry);
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
    carry = rAC >> REG_12BIT;  
    rAC = rAC << 1;
    rAC &= REG_12BIT_MASK;
    rAC += carry;

#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG: rotate left from %o to %o with a carry %o\n", 
            temp, rAC, carry);
#endif
}


//================================================================================== 
//Description: shifts rAC right by num amount
//================================================================================== 
void alu::shiftRight(unsigned short num)
{
    unsigned short temp = rAC;

    rAC = rAC >> num;
    rAC &= REG_12BIT_MASK;

#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG: shift right from %o to %o\n", temp, rAC);
#endif
}


//================================================================================== 
//Description: shifts rAC left by num amount
//================================================================================== 
void alu::shiftLeft(unsigned short  num)
{
    unsigned short temp = rAC;

    rAC = rAC << num;
    rAC &= REG_12BIT_MASK;

#ifdef DEBUG_ALU
    fprintf(stdout, "DEBUG: shift left from %o to %o\n", temp, rAC);
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
    fprintf(stdout, "DEBUG: and AC from %o to %o\n", temp, rAC);
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
    fprintf(stdout, "DEBUG: or AC from %o to %o\n", temp, rAC);
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


