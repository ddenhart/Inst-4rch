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

#ifndef __ACCUMULATOR_H_INCLUDED__
#define __ACCUMULATOR_H_INCLUDED__

//Dependencies
//================================================================================== 
class BitReg;


//Class Accumulator
//================================================================================== 
class Accumulator
{
private:

    bool carryOrOverflow();

public:
    Accumulator();
    ~Accumulator();
    void sum(BitReg* rInReg);
    void handleCarry();
    bool isNegative();
    bool isZero();
    void complement();
    void increment();
    void negate();
    void clear();
    void rotateRight();
    void rotateLeft();
};


#endif // __ACCUMULATOR_H_INCLUDED__

