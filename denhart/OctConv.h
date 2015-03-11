/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    OctConv.cpp
Date:			    03/02/2015
Description:	 This file contains the class OctConv
================================================================================== */

#ifndef __OCTCONV_H_INCLUDED__
#define __OCTCONV_H_INCLUDED__

//Defines
//================================================================================== 
//#define DEBUG_CONVERT 
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
#define HEX_4BIT 4
#define HEX_MAX 16


//Class OctConv
//================================================================================== 
class OctConv
{

private:

    char m_sOctTable[OCT_MAX];
    bool m_bOctTable[OCT_MAX][OCT_3BIT];
    char m_sHexTable[HEX_MAX];
    bool m_bHexTable[HEX_MAX][HEX_4BIT];

    int getLength(bool* inReg);
    int findsHex(char sIn);

public:

    OctConv();
    ~OctConv();
    char convToString(bool* inReg, int num);
    char convToString(unsigned int inReg);
    unsigned int convToNumber(bool* inReg);
    unsigned int convToNumber(char inReg);
    bool* convToBinary(char inReg);
    bool* convToBinary(unsigned int inReg);
    bool* convertToBinaryHex(char* inReg);
};


#endif //__OCTCONV_H_INCLUDED__

