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

#ifndef __alu_H_INCLUDED__
#define __alu_H_INCLUDED__

//Dependencies
//================================================================================== 

//Defines
//================================================================================== 
#define DEBUG_ALU 

//Class alu
//================================================================================== 
class alu
{
private:
    bool m_bZero;
    bool m_bNegative;
    bool rLB; // 1 bit link bit (carry out)
    unsigned short rAC; //12 bit alu
    //unsigned short rMQ; //12 bit multiplier quotient

public:
    alu();
    ~alu();
    unsigned short getAC();
    bool getLB();
    void setAC(unsigned short rac);
    void setLB(bool rac);
    void sumALU(unsigned short reg);
    bool isNegative();
    bool isZero();
    void complementLC();
    void complementALU();
    void incrementALU();
    void negateALU();
    void clear();
    void clearAC();
    void clearLC();
    void rotateRight();
    void rotateLeft();
    void shiftRight(unsigned short  num);
    void shiftLeft(unsigned short  num);
    void andALU(unsigned short reg);
    void orALU(unsigned short reg);
    void printAll();
    void printALU();
};


#endif 

// __alu_H_INCLUDED__

