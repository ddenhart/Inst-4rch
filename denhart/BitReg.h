/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
Edward Sayers
==================================================================================
File:			    BitReg.cpp
Date:			    03/02/2015
Description:	 This file contains the class BitReg
                A char represents 3 bits or 1 octal symbol
                A bool represents 1 bit 
                An array of bools represents a binary vector
                An unsigned int represents the decimal value of a binary vector
================================================================================== */

#ifndef __BITREG_H_INCLUDED__
#define __BITREG_H_INCLUDED__

//Dependencies
//================================================================================== 
class OctConv;

//Defines
//================================================================================== 
#define DEBUG_REG true
#define MASK0 0x00F
#define MASK1 0x0F0
#define MASK2 0xF00


//Class BitReg
//================================================================================== 
class BitReg
{
private:
    int m_iLength;  //length of the bit vector
    bool* m_rBitReg;  //bit vector representing a binary value
    
    void copyValue(bool* reg); //copies a bool array into the internal register
    void copyValue(char* reg); //copies a char array into the internal register
    void copyValue(unsigned int reg); //copies an unsigned into the internal register
    //copies the register value from rSource to BitReg
    void copyReg(BitReg* rSource, BitReg* rDest);
    //copies the register value from bool* rSource to BitReg
    void copyReg(bool* rSource, BitReg* rDest);
    //copies the register value from char* rSource to BitReg
    void copyReg(char* rSource, BitReg* rDest);
    //copies the register value from unsigned int rSource to BitReg
    void copyReg(unsigned int rSource, BitReg* rDest);
    //saves to the register 3 bits starting at index and increasing
    int save3bit(bool* bReg, int index);
        //gives number of bits needed to translate num to a binary vector 
        //int findNumBits(unsigned int num);
    //fills a regester given a total number of bits and an unsigned int value
    bool* fillRegister(int total, unsigned int value);
    //fills a regester given a total number of bits and a char array value
    bool* fillRegister(int total, char* value);
    //deletes the current register, resizes it to size and initializes with zeros
    void resizeRegister(int size);
    int outOfRangeChar(char num); //checks that num is an octal value
    unsigned int convertedNumb(bool* reg); //converts a boolean array to decimal
    int convertedNumb2sComp(bool* reg); //
    char* convertedChar(bool* reg); //converts a boolean array to char array
    char* convertedChar(bool* reg, int number); //converts a boolean array to char array
    char convertedBinary(bool reg); //converts a bool to the chars 0 or 1
    bool* bitChunk(int start, int length); //returns a section of bits from the register
    int zeroPad(bool* reg, int padding);

public:

    BitReg();  //constructor
    BitReg(int regSize); //constructor for empty register of input size
    BitReg(bool* rSource); //constructor with bool* value 
    BitReg(char* rSource); //constructor with char* value
    BitReg(unsigned int rSource); //constructor with an unsigned int value 
    BitReg(bool* rSource, int size); //constructor with bool* value and a given size
    BitReg(char* rSource, int size); //constructor with char* value and a given size
    BitReg(unsigned int rSource, int size); //constructor with an unsigned int value and a given size
    ~BitReg(); //deconstructor
    void initArray();//sets all bits of the register to 0
    void initArray(bool* reg, int size); //for temporaries
    void initArray(char* reg, int size);
    int getLength(); //returns the length of the boolean register
    int getLength(BitReg* inReg); //returns the length of the input register
    int getLength(bool* inReg); //returns the length of the boolean register
    int getLength(char* inReg); //returns the length of the char register (a char is 3 bits)
    int getLength(unsigned int inReg); //returns the length of the unsigned int, where int is in decimal
    void setSize(int num); //sets the size of a new register
    void setReg(BitReg* rInReg); //sets the register with another register
    void setReg(bool* rInReg); //sets the register with a bool array
    void setReg(char* rInReg); //sets the register with a char array
    void setReg(unsigned int rInReg); //sets the register with an unsigned int
    BitReg* getReg(); //returns a copy of the register
    bool* getBool(); //returns the value of the register
    unsigned int getNumber(); //returns the decimal value of the register
    unsigned int getNumber(bool* rInReg); //returns the decimal value of the bool array input
    unsigned int getNumber(char* rInReg); //returns the decimal value of the char array input
    int getNumber2sComp(); //returns the decimal value of the register
    int getNumber2sComp(bool* rInReg); //returns the decimal value of the bool array input
    int getNumber2sComp(char* rInReg); //returns the decimal value of the char array input
    char* getString();
    char* getString(int size);
    char* getString(BitReg* rInReg);
    char* getString(bool* rInReg);
    char* getString(unsigned int rInReg);
    BitReg* sliceBits(int start, int length);
    BitReg* ccatRegs(BitReg* rSource, int iNumberWords);
    void printRegister();
    char* getBinary();
    char* getBinary(bool* reg);
    void complement();
};


#endif // __BITREG_H_INCLUDED__

