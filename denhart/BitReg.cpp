/* ==================================================================================
ECE 486 / Winter 2015 / PDP-8 simulator project
Team:
Deborah Denhart
Jeremiah Franke
 
==================================================================================
File:			    BitReg.cpp
Date:			    03/02/2015
Description:	 This file contains the class BitReg
                A char represents 3 bits or 1 octal symbol
                A bool represents 1 bit
                An array of bools represents a binary vector
                An unsigned int represents the decimal value of a binary vector
================================================================================== */

#include <cstdio>
#include <cstring>
#include "Common.h"
#include "OctConv.h"
#include "BitReg.h"

//External objects
//================================================================================== 
extern ErrorTable Error;
extern OctConv OctTable;


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor
BitReg::BitReg()
{
    m_rBitReg = NULL;
    m_iLength = 0;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor for empty register of input size
BitReg::BitReg(int regSize)
{
    setSize(regSize);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor with bool
BitReg::BitReg(bool* rSource)
{
    if(rSource)
    {
        int size = getLength(rSource);
        setSize(size);
        copyValue(rSource);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor with char
BitReg::BitReg(char* rSource)
{
    if(rSource)
    {
        int size = getLength(rSource);
        setSize(size);
        copyValue(rSource);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor with int
BitReg::BitReg(unsigned int rSource)
{
    int size = getLength(rSource);
    setSize(size);
    copyValue(rSource);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor with bool* value and a given size
BitReg::BitReg(bool* rSource, int size)
{
#ifdef DEBUG_BITREG
	printf("rSrc: %d\n", rSource);
#endif
    if(rSource)
    {
        setSize(size);
        copyValue(rSource);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor with char* value and a given size
BitReg::BitReg(char* rSource, int size)
{
    if(rSource)
    {
        setSize(size);
        copyValue(rSource);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//constructor with an unsigned int value and a given size
BitReg::BitReg(unsigned int rSource, int size)
{
    setSize(size);
    copyValue(rSource);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//deconstructor
BitReg::~BitReg()
{
    if(m_rBitReg)
    {
        delete [] m_rBitReg;
        m_rBitReg = NULL;
    }

    m_iLength = 0;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//sets the size of a new register
void BitReg::setSize(int num)
{
    if(m_rBitReg)
    {
        delete[] m_rBitReg;
        m_rBitReg = NULL;
        m_iLength = 0;
    }

    if(num > 0)
    {
        m_iLength = num;
        m_rBitReg = new bool[num];
        initArray();
    }
    else
    {
        m_rBitReg = NULL;
        m_iLength = 0;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//copies a bool array into the internal register
void BitReg::copyValue(bool* reg)
{
    int length = getLength(reg);
    int offset = 0;

    if(reg)
    {
        //if the register size is too small, increase it
        if(m_iLength < length)
        {
            resizeRegister(length);
        }
        else if(m_iLength > length)
        {
            offset = m_iLength - length;
        }
        //else if equal no adjustment needed
        initArray();

        //copy length starting from LSB (m_iLength - 1)
#ifdef DEBUG_BITREG		
		printf("reg: ");
#endif		
        for(int i = 0; i < length; ++i)
        {
#ifdef DEBUG_BITREG		
			printf("%d", reg[i]);
#endif	
            m_rBitReg[offset] = reg[i];
            ++offset;
        }
#ifdef DEBUG_BITREG		
		printf("\n");
#endif	
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//copies a char array into the internal register
void BitReg::copyValue(char* reg)
{
    int length = getLength(reg);
    int total = length; 
    bool* temp = NULL;

    if(reg)
    {
        //if the register size is too small, increase it
        if(m_iLength < total)
        {
            resizeRegister(total);
        }
        else if(m_iLength > total)
        {
            total = m_iLength;
        }

        initArray();
        temp = fillRegister(total, reg);
        copyValue(temp);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
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
//copies an unsigned into the internal register
void BitReg::copyValue(unsigned int reg)
{
    int length = getLength(reg);
    bool* btemp = NULL;

    //if the register size is too small, increase it
    if(m_iLength < length)
    {
        resizeRegister(length);
    }
    else if(m_iLength > length)
    {
        length = m_iLength;
    }
   // else lengths are equal
    initArray();
    btemp = fillRegister(length, reg);
    copyValue(btemp);

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
//copies the register value from rSource to BitReg
//BitReg rSource holds the register value that will be coppied
//returns BitReg with coppied value
void BitReg::copyReg(BitReg* rSource, BitReg* rdest)
{
    if(rSource)
    {
        int iLength = getLength(rSource);
        //if rdest is NULL make a new rdest
        if(!rdest)
        {
            rdest = new BitReg(iLength);
        }
        //if rSource and rdest's lengths don't match, resize rdest
        int rLength = getLength(rdest);
        if(iLength != rLength)
        {
            rdest->resizeRegister(iLength);
        }
        initArray();
        //copy the register's values
        for(int i = 0; i < iLength; ++i)
        {
            if(rSource->m_rBitReg[i])
            {
                rdest->m_rBitReg[i] = rSource->m_rBitReg[i];
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_BITREG);
                break;
            }
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//copies the register value from rSource to BitReg
//bool* rSource holds the register value that will be coppied
//returns BitReg with coppied value
void BitReg::copyReg(bool* rSource, BitReg* rdest)
{
    BitReg temp(rSource);
    copyReg(&temp, rdest);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//copies the register value from rSource to BitReg
//char* rSource holds the register value that will be coppied
//returns BitReg with coppied value
void BitReg::copyReg(char* rSource, BitReg* rdest)
{
    BitReg temp(rSource);
    copyReg(&temp, rdest);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//copies the register value from rSource to BitReg
//unsigned int rSource holds the register value that will be coppied
//returns BitReg with coppied value
void BitReg::copyReg(unsigned int rSource, BitReg* rdest)
{
    BitReg temp(rSource);
    copyReg(&temp, rdest);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//==================================================================================
//fills a regester given a total number of bits and a value
//with the MSB at index 0 and the LSB at index total -1
bool* BitReg::fillRegister(int total, unsigned int value)
{
    bool* reg = NULL;
    int revIndex = total - 1;
    unsigned int number = 0;
    bool conv = 0;

    if(total > 0)
    {
        reg = new bool[total];
        initArray(reg, total);

        for(int i = 0; i < total; ++i)
        {
            if(revIndex < 0)
            {
                Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
                break;
            }
            else
            {
                number = value % 2;
                if(number)
                {
                    conv = 1;
                }
                else
                {
                    conv = 0;
                }

                reg[revIndex] = conv;
                --revIndex;
                value /= 2;
            }
        }
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
    }

    return reg;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//fills a register given a total number of bits and a char array value
bool* BitReg::fillRegister(int total, char* value)
{
    int index = 0;
    int bitlength = getLength(value); //the bits will be a multiple of 3 because a char is 3 bits
    int slength = bitlength/OCT_3BIT; //number of chars in the string
    bool* temp = NULL;
    bool* tempo = NULL;

    if(value)
    {
        if(0 < total)
        {
            //if the desired bits is less than value's converted equivallent bits, use total
            if(total < bitlength)
            {
                slength = total/ OCT_3BIT; //must be a multiple of 3
            }

            temp = new bool[total];
            initArray(temp, total);

            for(int i = 0; i < slength; ++i)
            {
                if(!outOfRangeChar(value[i]))
                {
                    tempo = OctTable.convToBinary(value[i]);
                    for(int j = 0; j < OCT_3BIT; ++j)
                    {
                        if(index < total)
                        {
                            temp[index] = tempo[j];
                            ++index;
                        }
                        else
                        {
                            Error.printError(ERROR_NULL, FILE_BITREG);
                            break;
                        }
                    }

                    if(tempo)
                    {
                        delete[] tempo;
                        tempo = NULL;
                    }
                }
                else
                {
                    Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
                    break;
                }
            }
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
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
//deletes the current register, resizes it to size and initializes with zeros
void BitReg::resizeRegister(int size)
{
    if(m_rBitReg)
    {
        delete[] m_rBitReg;
        m_rBitReg = NULL;
        m_iLength = 0;
    }

    if(size > 0)
    {
        m_iLength = size;
        m_rBitReg = new bool[m_iLength];
        initArray();
    }
    else
    {
        m_rBitReg = NULL;
        m_iLength = 0;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//checks that num is an octal value
//inputs a char value
//returns 0 if octal value, else a negative value
int BitReg::outOfRangeChar(char num)
{
    int error = 0;

    if((num < '0') || (num > '7'))
    {
        error = ERROR_OUT_OF_RANGE;
        Error.printError(error, FILE_BITREG);
    }

    return error;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//converts a boolean array to decimal
unsigned int BitReg::convertedNumb(bool* reg)
{
    unsigned int iTotal = 0;
    unsigned int pwr = 2;
    int length = 0;
    int index = 0;

    if(reg)
    {
        length = getLength(reg) - 1;
        index = length;

        if(length < 0)
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
        }
        else
        {
            //get LSB
            if(reg[index])
            {
                ++iTotal;
            }
            --index;

            //need to index backwards because the MSB is at 0
            //and LSB is m_iLength - 1
            for(int i = 0; i < length; ++i)
            {
                if(reg[index])
                {
                    iTotal += pwr;
                }
                pwr *= 2;
                --index;
            }
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    return iTotal;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
int BitReg::convertedNumb2sComp(bool* reg)
{
    unsigned int total = 0;
    int length = 0;
    int signIndex = 0;
    bool* temp = NULL;

    if(reg)
    {
        length = getLength(reg);

        if((length-2) < 0)
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
        }
        else
        {
            temp = new bool[length-1];
			bool check = reg[0];
			for (int i = 1; i < length; i++)
			{
				if (check)
				{
					if (reg[i] == true)
						reg[i] = false;
					else if (reg[i] == false)
						reg[i] = true;
				}
                temp[signIndex] = reg[i];
                ++signIndex;
            }
			if (check)
				total = convertedNumb(temp) + 1;
			else
				total = convertedNumb(temp);
			
            if(check) //if the signed bit is positive, negate
            {
                total *= -1;
            }
#ifdef DEBUG_REG
            fprintf(stdout, "DEBUG bitreg: %d\n", total);
#endif
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }

    return total;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void BitReg::complement()
{
    bool* temp = NULL;

    if(m_rBitReg)
    {
        if(m_iLength < 0)
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
        }
        else
        {
            temp = new bool[m_iLength];

            for(int i = 0; i < m_iLength; ++i)
            {
                temp[i] = !m_rBitReg[i];
            }
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    setReg(temp);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//converts a boolean array to char array
char* BitReg::convertedChar(bool* reg)
{
    int length = getLength(reg);
    int modCount = length % OCT_3BIT;
    int total = length/OCT_3BIT;
    int index = 0;
    int sIndex = 0;
    char* stemp = NULL;
    bool* btemp = NULL;
    bool* btempex = NULL;

    if(reg)
    {
        //get total number of chars
        if(modCount)
        {
            ++total;
        }
        stemp = new char[total +1];
        initArray(stemp, total);
        //make bits multiples of 3 to convert to a char
        //extend the MSB with 0's
        if(2 == modCount)
        {
            index = 1;
            btempex = bitChunk(reg, index, modCount);
            btemp = new bool[OCT_3BIT];
            initArray(btemp, OCT_3BIT);
            btemp[0] = 0;
            btemp[1] = btempex[0];
            btemp[2] = btempex[1];
            stemp[sIndex] = OctTable.convToString(btemp, OCT_3BIT);
            ++sIndex;
        }
        else if(1 == modCount)
        {
            index = 2;
            btempex = bitChunk(reg, index, modCount);
            btemp = new bool[OCT_3BIT];
            initArray(btemp, OCT_3BIT);
            btemp[0] = 0;
            btemp[1] = 0;
            btemp[2] = btempex[0];
            stemp[sIndex] = OctTable.convToString(btemp, OCT_3BIT);
            ++sIndex;
        }
        //else modcount is 0, no need to add extra bits

        if(btemp)
        {
            delete[] btemp;
            btemp = NULL;
        }
        index = modCount;

        for(int i = sIndex; i < total; ++i)
        {
            btemp = bitChunk(reg, index, OCT_3BIT);
            stemp[i] = OctTable.convToString(btemp, OCT_3BIT);
            index += OCT_3BIT;
        }
        stemp[total] = '\0';
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    if(btempex)
    {
        delete [] btempex;
        btempex = NULL;
    }
    if(btemp)
    {
        delete [] btemp;
        btemp = NULL;
    }

    return stemp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//converts a boolean array to char array
char* BitReg::convertedChar(bool* reg, int length)
{
    //int length = getLength(reg);
    int modCount = length % OCT_3BIT;
    int total = length/OCT_3BIT;
    int index = 0;
    int sIndex = 0;
    char* stemp = NULL;
    bool* btemp = NULL;
    bool* btempex = NULL;

    if(reg)
    {
        //get total number of chars
        if(modCount)
        {
            ++total;
        }
        stemp = new char[total +1];
        initArray(stemp, total);
        //make bits multiples of 3 to convert to a char
        //extend the MSB with 0's
        if(2 == modCount)
        {
            //index = 1;
            btempex = bitChunk(reg, index, modCount);
            btemp = new bool[OCT_3BIT];
            initArray(btemp, OCT_3BIT);
            btemp[0] = 0;
            btemp[1] = btempex[0];
            btemp[2] = btempex[1];
            stemp[sIndex] = OctTable.convToString(btemp, OCT_3BIT);
            ++sIndex;
        }
        else if(1 == modCount)
        {
            //index = 2;
            btempex = bitChunk(reg, index, modCount);
            btemp = new bool[OCT_3BIT];
            initArray(btemp, OCT_3BIT);
            btemp[0] = 0;
            btemp[1] = 0;
            btemp[2] = btempex[0];
            stemp[sIndex] = OctTable.convToString(btemp, OCT_3BIT);
            ++sIndex;
        }
        //else modcount is 0, no need to add extra bits

        if(btemp)
        {
            delete[] btemp;
            btemp = NULL;
        }

	    if(btempex)
	    {
	        delete[] btempex;
	        btempex = NULL;
	    }
	    index = modCount;

        for(int i = sIndex; i < total; ++i)
        {
            btemp = bitChunk(reg, index, OCT_3BIT);
            stemp[i] = OctTable.convToString(btemp, OCT_3BIT);
            index += OCT_3BIT;

            if(btemp)
            {
                delete[] btemp;
                btemp = NULL;
            }
        }
        stemp[total] = '\0';
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    return stemp;

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//converts a bool to the chars 0 or 1
char BitReg::convertedBinary(bool reg)
{
    char temp = '0';
    if(reg)
    {
        temp = '1';
    }
    //else default value of temp, 0

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns a section of bits from the register
bool* BitReg::bitChunk(int start, int length)
{
    int index = 0;
    int end = start + length;
    bool* temp = NULL;

    if((start >= 0) && (length > 0) && (end <= m_iLength))
    {
        temp = new bool[length];
        initArray(temp, length);
        for(int i = start; i < end; ++i)
        {
            if(index < length)
            {
                temp[index] = m_rBitReg[i];
                ++index;
            }
            else
            {
                Error.printError(ERROR_NULL, FILE_BITREG);
            }
        }
    }
    else
    {
        Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
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
//returns a section of bits from the register
bool* BitReg::bitChunk(bool* reg, int start, int length)
{
    int mlength = 0;
    int index = 0;
    int end = start + length;
    bool* temp = NULL;

    if(reg)
    {
        mlength = getLength(reg);
        if((start >= 0) && (length > 0) && (end <= mlength))
        {
            temp = new bool[length];
            initArray(temp, length);
            for(int i = start; i < end; ++i)
            {
                if(index < length)
                {
                    temp[index] = reg[i];
                    ++index;
                }
                else
                {
                    Error.printError(ERROR_NULL, FILE_BITREG);
                }
            }
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
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
int BitReg::zeroPad(bool* reg, int padding)
{
    int i = 0;

    if(reg)
    {
        //set leading 0's
        for(int i = 0; i < padding; ++i)
        {
            reg[i] = 0;
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
    return i;

}


//================================================================================== 
//Name:
//Description:sets all bits of the register to 0
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void BitReg::initArray()
{
    if(m_rBitReg)
    {
        for(int i = 0; i < m_iLength; ++i)
        {
            m_rBitReg[i] = 0;
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void BitReg::initArray(bool* reg, int size)
{
    if(reg)
    {
        for(int i = 0; i < size; ++i)
        {
            reg[i] = 0;
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void BitReg::initArray(char* reg, int size)
{
    if(reg)
    {
        for(int i = 0; i < size; ++i)
        {
            reg[i] = '0';
        }
        reg[size-1] = '\0';
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns the length of the boolean register
int BitReg::getLength()
{
    return m_iLength;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
 //returns the length of the input register
 int BitReg::getLength(BitReg* rInReg)
 {
     int temp = 0;
     bool* btemp = NULL;

     if(rInReg)
     {
         temp = rInReg->m_iLength;
     }
     else
     {
         Error.printError(ERROR_NULL, FILE_BITREG);
     }

     if(btemp)
     {
         delete [] btemp;
         btemp = NULL;
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
 //returns the length of the boolean register
 int BitReg::getLength(bool* rInReg)
 {
     int count = 0;
     int conv = 0;
     bool done = false;
     
     if(rInReg)
     {
         while(!done)
         {
            conv = rInReg[count];
            if((conv == 0) || (conv == 1))
            {
                ++count;
            }
            else
            {
                done = true;
            }
         }
     }
     else
     {
         Error.printError(ERROR_NULL, FILE_BITREG);
     }

     return count;
 }


 //================================================================================== 
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //returns the length of the char register (a char is 3 bits)
 int BitReg::getLength(char* rInReg)
 {
     int count = 0;
     
     if(rInReg)
     {
         while(rInReg[count])// != '\0')
         {
             ++count;
         }

         //count = sizeof(rInReg)/sizeof(char*); //number of chars
         count *= 3; //convert to bits
     }
     else
     {
         Error.printError(ERROR_NULL, FILE_BITREG);
     }

     return count;
 }


 //================================================================================== 
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //returns the length of the unsigned int, where int is in decimal
 int BitReg::getLength(unsigned int rInReg)
 {
     int count = 0;
     int remainder = rInReg;
     

     //find number of bits
     while(remainder)
     {
         ++count;
         remainder /= 2;
     }

     if(0 == count)
     {
         ++count;
     }
     //account for the one's bit place at the MSB
     //++count;

     return count;
 }


 //================================================================================== 
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
//sets the register value with input value
//BitReg rInReg holds the register value that will be set
//returns standard error
void BitReg::setReg(BitReg* rInReg)
{
    bool* temp = NULL;

    if(rInReg)
    {
        temp = rInReg->getBool();
        if(temp)
        {
            copyValue(temp);
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
    
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//sets the register value
//bool* rInReg should be sent is as bool rInReg[12]
//returns standard error
void BitReg::setReg(bool* rInReg)
{
    if(rInReg)
    {
        copyValue(rInReg);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//sets a register to an array of chars
void BitReg::setReg(char* rInReg)
{
    if(rInReg)
    {
        copyValue(rInReg);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void BitReg::setReg(unsigned int rInReg)
{
    copyValue(rInReg);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns a copy of the register
BitReg* BitReg::getReg()
{
    //return temp, so the original register isn't changed
    BitReg* temp = new BitReg(m_rBitReg, m_iLength);
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns a boolean array
bool* BitReg::getBool()
{
    //return temp, so the original register isn't changed
    bool* temp = new bool[m_iLength];
    initArray(temp, m_iLength);

    for(int i = 0; i < m_iLength; ++i)
    {
        temp[i] = m_rBitReg[i];
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
//returns an unsigned integer conversion of the boolean array
unsigned int BitReg::getNumber()
{
    unsigned int temp = 0;
    temp = convertedNumb(m_rBitReg);
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//converts an input boolean array to an unsigned integer
unsigned int BitReg::getNumber(bool* rInReg)
{
    unsigned int temp = 0;

    if(rInReg)
    {
        temp = convertedNumb(rInReg);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
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
//converts an input char array to an unsigned integer
unsigned int BitReg::getNumber(char* rInReg)
{
    bool* temp = NULL;
    int size = 0;
    unsigned int itemp = 0;

    if(rInReg)
    {
        size = getLength(rInReg);
        temp = fillRegister(size, rInReg);
        if(temp)
        {
            itemp = convertedNumb(temp);
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

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }

    return itemp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns an integer conversion of the boolean array
int BitReg::getNumber2sComp()
{
    int temp = 0;
    temp = convertedNumb2sComp(m_rBitReg);
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns an integer conversion of the boolean array
int BitReg::getNumber2sComp(bool* rInReg)
{
    int temp = 0;
    temp = convertedNumb2sComp(rInReg);
    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns an integer conversion of the boolean array
int BitReg::getNumber2sComp(char* rInReg)
{
    bool* temp = NULL;
    int size = 0;
    int itemp = 0;

    if(rInReg)
    {
        size = getLength(rInReg);
        temp = fillRegister(size, rInReg);
        if(temp)
        {
            itemp = convertedNumb2sComp(temp);
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

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }

    return itemp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
char* BitReg::getString()
{
    char* temp = NULL;

    if(m_rBitReg)
    {
        temp = convertedChar(m_rBitReg, m_iLength);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
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
char* BitReg::getString(int size)
{
    char* temp = NULL;
    int bitslength = size*3; //3 bits per char
    int bitOffset = 0;
    int index = 0;
    bool* reg = NULL;

    if(size > 0)
    { 
        //if there isn't alocated space for the register yet, make a new array
        //size is 0 in this case
        if(!m_rBitReg)
        {
            m_rBitReg = new bool[bitslength];
            m_iLength = size;
            initArray();
        }

        //check if leading 0's will be necessary
        if(bitslength > m_iLength)
        {
            bitOffset = bitslength - m_iLength;
            reg = new bool[bitslength];
            initArray(reg, bitslength);
            index = zeroPad(reg, bitOffset);

            //copy the rest of the array
            for(int j = 0; j < m_iLength; ++j)
            {
                reg[index] = m_rBitReg[j];
                ++index;
            }
        }
        else if(bitslength < m_iLength) //chec if less bits are requested
        {
            bitOffset = m_iLength - bitslength;
            reg = bitChunk(bitOffset, bitslength);
        }
        else //the registers are equal
        {
            reg = new bool[m_iLength];
            for(int i = 0; i < m_iLength; ++i)
            {
                reg[i] = m_rBitReg[i];
            }
        }
    }

    temp = convertedChar(reg, bitslength);

    if(reg)
    {
        delete[] reg;
        reg = NULL;
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
char* BitReg::getString(BitReg* rInReg)
{
    char* temp = NULL;
    bool* btemp = NULL;

    if(rInReg)
    {
        btemp = rInReg->getBool();
        if(btemp)
        {
            temp = convertedChar(btemp);
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

    if(btemp)
    {
        delete[] btemp;
        btemp = NULL;
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
char* BitReg::getString(bool* rInReg)
{
    return convertedChar(rInReg);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
char* BitReg::getString(unsigned int rInReg)
{
    int length = getLength(rInReg);
    bool* temp = fillRegister(length, rInReg);
    return convertedChar(temp);
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//copies the register value from rSource to rDest
//BitReg rSource holds the register value that will be coppied, sent in as rSource
//int start is the starting index
//int length is the length of chunk desired
//returns standard error
BitReg* BitReg::sliceBits(int start, int length)
{
    bool* temp = bitChunk(start, length);
    BitReg* rDest = new BitReg(temp);
    return rDest;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//concatenates octal words
//BitReg* rSource is an array of BitReg
//      to be concatenated, rSource[0] = MSB, rSource[max] = LSB
//int iNumberWords are number of BitReg being sent
//returns register of concatenated values
BitReg* BitReg::ccatRegs(BitReg* rSource, int iNumberWords)
{
    int index = 0;
    int iBitLength = 0;
    int length = 0;
    bool* bOut = NULL;
    bool* temp = NULL;
    BitReg* rOut = NULL; //base constructor sets length to 0 and register to null

    if(rSource)
    {
        for(int k = 0; k < iNumberWords; ++k)
        {
            iBitLength += rSource[k].getLength();
        }

        if(iBitLength > 0)
        {
            bOut = new bool[iBitLength]; 
            initArray(bOut, iBitLength);

            for(int i = 0; i < iNumberWords; ++i)
            {
                length = rSource[i].getLength();
                temp = rSource[i].getBool();
                
                if(temp)
                {
                    for(int j = 0; j < length; ++j)
                    {
                        if(index < iBitLength)
                        {
                            bOut[index] = temp[j];
                            ++index;
                        }
                        else
                        {
                            Error.printError(ERROR_NULL, FILE_BITREG);
                            break;
                        }
                    }
                }
            }
            rOut = new BitReg(bOut);
        }
        else
        {
            Error.printError(ERROR_OUT_OF_RANGE, FILE_BITREG);
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_BITREG);
    }

    if(temp)
    {
        delete[] temp;
        temp = NULL;
    }

    return rOut;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
void BitReg::printRegister()
{
    BitReg* temp[2];
    BitReg* temp0 = getReg();
    BitReg* temp1 = sliceBits(0, 3);
    temp[0] = temp0;
    temp[1] = temp1;
    BitReg* temp2 = ccatRegs(temp[0], 2);
    int iLenth = getLength();
    bool* bBool =  getBool();
    unsigned int iNumber = getNumber();
    char* sString = getString();
    int iLenthr = getLength(temp0);
    int iLenthb = getLength(bBool);
    int iLengths = getLength(sString);
    int iLengthi = getLength(iNumber);
    unsigned int iNumberb = getNumber(bBool);
    unsigned int iNumbers = getNumber(sString);
    char* sStringr = getString(temp0);
    char* sStringb = getString(bBool);
    char* sStringi = getString(iNumber);

    fprintf(stdout, "%s\n", PRINT_BREAK);
    fprintf(stdout, "Length: %d, R: %d, B: %d, S: %d, I: %d\n", iLenth, iLenthr, iLenthb, iLengths, iLengthi);
    fprintf(stdout, "Bool: %s\n", getBinary(bBool));
    fprintf(stdout, "Number: %d, B: %d, S: %d\n", iNumber, iNumberb, iNumbers);
    fprintf(stdout, "String: %s, R: %s, B: %s, I: %s\n", sString, sStringr, sStringb, sStringi);
    if(temp0 && temp1 && temp2)
    {
        fprintf(stdout, "Register: %s, slice: %s, concat: %s\n", temp0->getBinary(), temp1->getBinary(), temp2->getBinary());
    }

    if(temp0)
    {
        delete temp0;
        temp0 = NULL;
    }
    if(temp)
    {
        delete temp1;
        temp1 = NULL;
    }
    if(temp2)
    {
        delete temp2;
        temp2 = NULL;
    }
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
char* BitReg::getBinary()
{
    int size = m_iLength;
    char* temp = new char[size +1];
    initArray(temp, size);

    for(int i = 0; i < size; ++i)
    {
        temp[i] = convertedBinary(m_rBitReg[i]);
    }
    temp[size] = '\0';

    return temp;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
char* BitReg::getBinary(bool* reg)
{
    int size = m_iLength;
    char* temp = NULL;

    if(size > 0)
    {
        temp = new char[size +1];
        initArray(temp, size);

        for(int i = 0; i < size; ++i)
        {
            temp[i] = convertedBinary(reg[i]);
        }
        temp[size] = '\0';
    }

    return temp;
}

