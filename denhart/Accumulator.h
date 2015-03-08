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

//Defines
//================================================================================== 
#define DEBUG_ALU true

//Class Accumulator
//================================================================================== 
class Accumulator
{
private:
    bool m_bZero;
    bool m_bNegative;

    void setAC(BitReg* rac);
    void setLB(BitReg* rac);
    void setCarry(bool* btemp);

public:
    Accumulator();
    ~Accumulator();
    BitReg* getAC();
    BitReg* getLB();
    void sumReg(BitReg* rInReg);
    bool isNegative();
    bool isZero();
    void complement();
    void increment(BitReg* rInReg);
    void negate();
    void clear();
    void rotateRight();
    void rotateLeft();
    void shiftRight(int num);
    void shiftLeft(int num);
    void andReg(BitReg* rInReg);
    void orReg(BitReg* rInReg);
    void printAll();
    void printALU();
};


#endif // __ACCUMULATOR_H_INCLUDED__

