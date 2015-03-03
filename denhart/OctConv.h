/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    OctConv.cpp
Date:			    03/02/2015
Description:	 This file contains the class OctConv
================================================================================== */

#ifndef __OCTCONV_H_INCLUDED__
#define __OCTCONV_H_INCLUDED__

//Defines
//================================================================================== 
#define OCT_MAX 8
#define OCT_0 0
#define OCT_1 1
#define OCT_2 2
#define OCT_3 3
#define OCT_4 4
#define OCT_5 5
#define OCT_6 6
#define OCT_7 7
#define OCT_3BIT 3


//Class OctConv
//================================================================================== 
class OctConv
{

private:

    unsigned int m_iOctTable[OCT_MAX];
    char m_sOctTable[OCT_MAX];
    bool m_bOctTable[OCT_MAX][OCT_3BIT];

    int getLength(bool* inReg);

public:

    OctConv();
    ~OctConv();
    int convToString(bool* inReg, char* rOutReg);
    char convToString(bool* inReg, int num);
    int convToString(unsigned int inReg, char* rOutReg);
    char convToString(unsigned int inReg);
    int convToNumber(bool* inReg, unsigned int* rOutReg);
    unsigned int convToNumber(bool* inReg);
    int convToNumber(char inReg, unsigned int* rOutReg);
    unsigned int convToNumber(char inReg);
    int convToBinary(char inReg, bool* rOutReg);
    bool* convToBinary(char inReg);
    int convToBinary(unsigned int inReg, bool* rOutReg);
    bool* convToBinary(unsigned int inReg);
};


#endif //__OCTCONV_H_INCLUDED__

