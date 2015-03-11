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

#include <cstdio>
#include <cstring>
#include <cctype>
#include "Common.h"
#include "BitReg.h"
#include "OctConv.h"

//External objects
//================================================================================== 
extern ErrorTable Error;


//==================================================================================
//Name: OctConv
//Description: constructor
//Inputs: none
//Outputs: none 
//Return: none
//==================================================================================
OctConv::OctConv()
{
    m_sOctTable[0] = '0';
    m_sOctTable[1] = '1';
    m_sOctTable[2] = '2';
    m_sOctTable[3] = '3';
    m_sOctTable[4] = '4';
    m_sOctTable[5] = '5';
    m_sOctTable[6] = '6';
    m_sOctTable[7] = '7';

    m_bOctTable[0][0] = 0;
    m_bOctTable[0][1] = 0;
    m_bOctTable[0][2] = 0;

    m_bOctTable[1][0] = 0;
    m_bOctTable[1][1] = 0;
    m_bOctTable[1][2] = 1;

    m_bOctTable[2][0] = 0;
    m_bOctTable[2][1] = 1;
    m_bOctTable[2][2] = 0;

    m_bOctTable[3][0] = 0;
    m_bOctTable[3][1] = 1;
    m_bOctTable[3][2] = 1;

    m_bOctTable[4][0] = 1;
    m_bOctTable[4][1] = 0;
    m_bOctTable[4][2] = 0;

    m_bOctTable[5][0] = 1;
    m_bOctTable[5][1] = 0;
    m_bOctTable[5][2] = 1;

    m_bOctTable[6][0] = 1;
    m_bOctTable[6][1] = 1;
    m_bOctTable[6][2] = 0;

    m_bOctTable[7][0] = 1;
    m_bOctTable[7][1] = 1;
    m_bOctTable[7][2] = 1;


    m_sHexTable[0] = '0';
    m_sHexTable[1] = '1';
    m_sHexTable[2] = '2';
    m_sHexTable[3] = '3';
    m_sHexTable[4] = '4';
    m_sHexTable[5] = '5';
    m_sHexTable[6] = '6';
    m_sHexTable[7] = '7';
    m_sHexTable[8] = '8';
    m_sHexTable[9] = '9';
    m_sHexTable[10] = 'A';
    m_sHexTable[11] = 'B';
    m_sHexTable[12] = 'C';
    m_sHexTable[13] = 'D';
    m_sHexTable[14] = 'E';
    m_sHexTable[15] = 'F';

    m_bHexTable[0][0] = 0;
    m_bHexTable[0][1] = 0;
    m_bHexTable[0][2] = 0;
    m_bHexTable[0][3] = 0;

    m_bHexTable[1][0] = 0;
    m_bHexTable[1][1] = 0;
    m_bHexTable[1][2] = 0;
    m_bHexTable[1][3] = 1;

    m_bHexTable[2][0] = 0;
    m_bHexTable[2][1] = 0;
    m_bHexTable[2][2] = 1;
    m_bHexTable[2][3] = 0;

    m_bHexTable[3][0] = 0;
    m_bHexTable[3][1] = 0;
    m_bHexTable[3][2] = 1;
    m_bHexTable[3][3] = 1;

    m_bHexTable[4][0] = 0;
    m_bHexTable[4][1] = 1;
    m_bHexTable[4][2] = 0;
    m_bHexTable[4][3] = 0;

    m_bHexTable[5][0] = 0;
    m_bHexTable[5][1] = 1;
    m_bHexTable[5][2] = 0;
    m_bHexTable[5][3] = 1;

    m_bHexTable[6][0] = 0;
    m_bHexTable[6][1] = 1;
    m_bHexTable[6][2] = 1;
    m_bHexTable[6][3] = 0;

    m_bHexTable[7][0] = 0;
    m_bHexTable[7][1] = 1;
    m_bHexTable[7][2] = 1;
    m_bHexTable[7][3] = 1;

    m_bHexTable[8][0] = 1;
    m_bHexTable[8][1] = 0;
    m_bHexTable[8][2] = 0;
    m_bHexTable[8][3] = 0;

    m_bHexTable[9][0] = 1;
    m_bHexTable[9][1] = 0;
    m_bHexTable[9][2] = 0;
    m_bHexTable[9][3] = 1;

    m_bHexTable[10][0] = 1;
    m_bHexTable[10][1] = 0;
    m_bHexTable[10][2] = 1;
    m_bHexTable[10][3] = 0;

    m_bHexTable[11][0] = 1;
    m_bHexTable[11][1] = 0;
    m_bHexTable[11][2] = 1;
    m_bHexTable[11][3] = 1;

    m_bHexTable[12][0] = 1;
    m_bHexTable[12][1] = 1;
    m_bHexTable[12][2] = 0;
    m_bHexTable[12][3] = 0;

    m_bHexTable[13][0] = 1;
    m_bHexTable[13][1] = 1;
    m_bHexTable[13][2] = 0;
    m_bHexTable[13][3] = 1;

    m_bHexTable[14][0] = 1;
    m_bHexTable[14][1] = 1;
    m_bHexTable[14][2] = 1;
    m_bHexTable[14][3] = 0;

    m_bHexTable[15][0] = 1;
    m_bHexTable[15][1] = 1;
    m_bHexTable[15][2] = 1;
    m_bHexTable[15][3] = 1;
}


//==================================================================================
//Name: OctConv
//Description: deconstructor
//Inputs: none
//Outputs: none 
//Return: none
//==================================================================================
OctConv::~OctConv()
{

}


//==================================================================================
//Name: getLength
//Description: gets the length of the boolean register
//Inputs: a boolean array
//Outputs: none
//Return: the length of the array
//==================================================================================
int OctConv::getLength(bool* rInReg)
{
    int count = 0;
    bool done = false;
    int conv = 0;

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
        Error.printError(ERROR_NULL, FILE_CONV);
    }

    return count;
}
//==================================================================================
//Name: findsHex
//Description: finds a hex equivalent for the input symbolic char
//Inputs: symbolic char between 0-F
//Outputs: none
//Return: integer value of the hex input between 0-15
//==================================================================================
int OctConv::findsHex(char sIn)
{
    int i = 0;
    int res = -1;
    char conv = toupper(sIn);

    for(i = 0; i < HEX_MAX; ++i)
    {
        if(conv == m_sHexTable[i])
        {
            res = i;
            break;
        }
    }

    return res;
}


 //==================================================================================
//Name: convToString
//Description: converts a binary value to a symbolic char
//Inputs:a boolean array representing binary, has a desired bit size of num
//Outputs: none
//Return: a char with a symbolic value between 0 and 8
 //==================================================================================
 char OctConv::convToString(bool* rInReg, int num)
 {
     int index = 0;
     int col = 0;
     bool bFound = false;
     char rOutReg = '0';

     if((num <= OCT_3BIT) && (num >= 0) && (rInReg))
     {
         for(int i = 0; i < OCT_MAX; ++i)
         {
             if(rInReg[0] == m_bOctTable[i][0])
             {
                 if(1 < num)
                 {
                     if(rInReg[1] == m_bOctTable[i][1])
                     {
                         if(2 < num)
                         {
                             if(rInReg[2] == m_bOctTable[i][2])
                             {
                                bFound = true;
                                index = i;
                                break;
                             }
                             else
                             {
                                 col = 0;
                             }
                         }
                         else
                         {
                             bFound = true;
                             index = i;
                             break;
                         }
                     }
                     else
                     {
                         col = 0;
                     }
                 }
                 else
                 {
                     bFound = true;
                     index = i;
                     break;
                 }
             }
             else//else not this row, keep looking
             {
                 col = 0;
             }
         }

         if(bFound)
         {
             rOutReg = m_sOctTable[index];
         }
         else
         {
             Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONV);
         }
     }
     else
     {
         Error.printError(ERROR_OUT_OF_RANGE, FILE_CONV);
     }

     return rOutReg;
 }


 //==================================================================================
 //Name: convToString
 //Description: converts an unsigned int to a symbolic char
 //Inputs:an unsigned int between 0 and 8 value
 //Outputs: none
 //Return: a char with a symbolic value between 0 and 8
 //==================================================================================
 char OctConv::convToString(unsigned int rInReg)
 {
     char rOutReg = '0';

     if(rInReg < OCT_MAX)
     {
         if(rOutReg)
         {
             rOutReg = m_sOctTable[rInReg];
         }
         else
         {
             Error.printError(ERROR_NULL, FILE_CONV);
         }
     }
     else
     {
         Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONV);
     }

     return rOutReg;

 }


 //==================================================================================
 //Name:convToNumber
 //Description: converts a bool array to its decimal equivalent
 //Inputs: a boolean array of a binary representation
 //Outputs: none
 //Return: an unsigned int value between 0 and 8
 //==================================================================================
 unsigned int OctConv::convToNumber(bool* rInReg)
 {
     int iLength = getLength(rInReg);
     int i = 0;
     int col = 0;
     bool bFound = false;
     unsigned int rOutReg = 0;

     if(iLength == OCT_3BIT)
     {
         for(i = 0; i < OCT_MAX; ++i)
         {
             if(rInReg[0] == m_bOctTable[i][0])
             {
                 if(rInReg[1] == m_bOctTable[i][1])
                 {
                     if(rInReg[2] == m_bOctTable[i][2])
                     {
                        bFound = true;
                     }
                     else
                     {
                         col = 0;
                     }
                 }
                 else
                 {
                     col = 0;
                 }
             } //else not this row, keep looking
             else
             {
                 col = 0;
             }
         }

         if(bFound)
         {
            rOutReg = i;
         }
         else
         {
             Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONV);
         }
     }
     else
     {
         Error.printError(ERROR_OUT_OF_RANGE, FILE_CONV);
     }

     return rOutReg;
 }


 //==================================================================================
 //Name: convToNumber
 //Description: converts a char to its decimal equivalent
 //Inputs: a char symbol between 0 and 8
 //Outputs: none
 //Return: an unsigned int value between 0 and 8
 //==================================================================================
 unsigned int OctConv::convToNumber(char rInReg)
 {
     int i = 0;
     bool bFound = false;
     unsigned int rOutReg = 0;

     for(i = 0; i < OCT_MAX; ++i)
     {
         if(rInReg == m_sOctTable[i])
         {
             bFound = true;
         } //else not this row, keep looking
     }

     if(bFound)
     {
         if(rOutReg)
         {
             rOutReg = i;// m_iOctTable[i];
         }
         else
         {
             Error.printError(ERROR_NULL, FILE_CONV);
         }
     }
     else
     {
         Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONV);
     }

     return rOutReg;
 }


 //==================================================================================
 //Name: convToBinary
 //Description: converts a cstring to it's binary equivalent
 //Inputs: char array to be converted
 //Outputs: none
 //Return: a 3 bit binary value in a boolean array
 //==================================================================================
 bool* OctConv::convToBinary(char rInReg)
 {
     int i = 0;
     bool bFound = false;
     bool* rOutReg = NULL;

     for(i = 0; i < OCT_MAX; ++i)
     {
         if(rInReg == m_sOctTable[i])
         {
             bFound = true;
             break;
         } //else not this row, keep looking
     }

     if(bFound)
     {
        rOutReg = new bool[OCT_3BIT];
        for(int j = 0; j < OCT_3BIT; ++j)
        {
            rOutReg[j] = m_bOctTable[i][j];
        }
     }
     else
     {
         Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONV);
     }

     return rOutReg;
 }


 //==================================================================================
 //Name: convToBinary
 //Description: converts an unsigned int to it's binary equivalent
 //Inputs: decimal to be converted
 //Outputs: none
 //Return: a 3 bit binary value in a boolean array
 //==================================================================================
 bool* OctConv::convToBinary(unsigned int rInReg)
 {
     bool* rOutReg = new bool[OCT_MAX];

     if(rInReg < OCT_MAX) 
     {
         if(rOutReg)
         {
             for(int j = 0; j < OCT_3BIT; ++j)
             {
                 if(rOutReg[j])
                 {
                     rOutReg[j] = m_bOctTable[rInReg][j];
                 }
                 else
                 {
                     Error.printError(ERROR_OUT_OF_RANGE, FILE_CONV);
                     break;
                 }
             }
         }
         else
         {
             Error.printError(ERROR_NULL, FILE_CONV);
         }
     }
     else
     {
         Error.printError(ERROR_UNEXPECTED_VALUE, FILE_CONV);
     }

     return rOutReg;
 }


//==================================================================================
 //Name: convertToBinaryHex
 //Description: converts a cstring to a 4 bit binary value per character,
 //                 characters are hexadecimal equivalents 0-F
 //Inputs: char array of hexadecimal characters
 //Outputs: none
 //Return: a boolean array of the binary equivalent of the hex char string
 //==================================================================================
bool* OctConv::convertToBinaryHex(char* inReg)
 {
    int length = 0;
    int index = 0;
    int tempIndex = 0;
    int size = 0;
    bool* temp = NULL;

    if(inReg)
    {
        length = strlen(inReg);
        --length; //don't include the '\0'
        size = HEX_4BIT*length; //get the binary array length
        temp = new bool[size];

        for(int i = 0; i < length; ++i)
        {
            index = findsHex(inReg[i]); // get a 4 bit equivalent of the input char
            if((index >= 0) && (index < HEX_MAX))
            {
                //copy hex value
                for(int j = 0; j < HEX_4BIT; ++j)
                {
                    if(tempIndex < size)
                    {
                        //copy the bits to the array
                        temp[tempIndex] = m_bHexTable[index][j]; 
                        ++tempIndex;
                    }
                    else
                    {
                        Error.printError(ERROR_OUT_OF_RANGE, FILE_CONV);
                    }
                }
            }
        }
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONV);
    }
     
    return temp;
 }

