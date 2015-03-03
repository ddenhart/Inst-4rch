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

#include <cstdio>
#include "Common.h"
#include "OctConv.h"

//External objects
//================================================================================== 
extern ErrorTable Error;


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OctConv::OctConv()
{
    m_iOctTable[0] = OCT_0;
    m_iOctTable[1] = OCT_1;
    m_iOctTable[2] = OCT_2;
    m_iOctTable[3] = OCT_3;
    m_iOctTable[4] = OCT_4;
    m_iOctTable[5] = OCT_5;
    m_iOctTable[6] = OCT_6;
    m_iOctTable[7] = OCT_7;

    m_sOctTable[0] = '0';
    m_sOctTable[1] = '1';
    m_sOctTable[2] = '2';
    m_sOctTable[3] = '3';
    m_sOctTable[4] = '4';
    m_sOctTable[5] = '5';
    m_sOctTable[6] = '6';
    m_sOctTable[7] = '7';
    m_sOctTable[8] = '8';

    m_bOctTable[0][0] = 0;
    m_bOctTable[0][1] = 0;
    m_bOctTable[0][2] = 0;

    m_bOctTable[1][0] = 1;
    m_bOctTable[1][1] = 0;
    m_bOctTable[1][2] = 0;

    m_bOctTable[2][0] = 0;
    m_bOctTable[2][1] = 1;
    m_bOctTable[2][2] = 0;

    m_bOctTable[3][0] = 1;
    m_bOctTable[3][1] = 1;
    m_bOctTable[3][2] = 0;

    m_bOctTable[4][0] = 0;
    m_bOctTable[4][1] = 0;
    m_bOctTable[4][2] = 1;

    m_bOctTable[5][0] = 1;
    m_bOctTable[5][1] = 0;
    m_bOctTable[5][2] = 1;

    m_bOctTable[6][0] = 0;
    m_bOctTable[6][1] = 1;
    m_bOctTable[6][2] = 1;

    m_bOctTable[7][0] = 1;
    m_bOctTable[7][1] = 1;
    m_bOctTable[7][2] = 1;
}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
OctConv::~OctConv()
{

}


//================================================================================== 
//Name:
//Description:
//Inputs:
//Outputs:
//Return:
//================================================================================== 
//returns the length of the boolean register
int OctConv::getLength(bool* rInReg)
{
    int count = 0;

    if(rInReg)
    {
        count = sizeof(rInReg)/sizeof(bool*);
    }
    else
    {
        Error.printError(ERROR_NULL, FILE_CONV);
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
//must be a 3 bit value, returns a value between 0 and 8
 int OctConv::convToString(bool* rInReg, char* rOutReg)
 {
    int error = ERROR_NONE;
    int iLength = getLength(rInReg);
    int i = 0;
    int col = 0;
    bool bFound = false;

    if(iLength == OCT_3BIT)
    {
        for(i = 0; i < OCT_MAX; ++i)
        {
            if(rInReg[0] == m_bOctTable[i][col++]) //check column 0
            {
                if(rInReg[1] == m_bOctTable[i][col++]) //check column 1
                {
                    if(rInReg[2] == m_bOctTable[i][col]) //check column 2
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
            if(rOutReg)
            {
                *rOutReg = m_sOctTable[i];  //copy the character
            }
            else
            {
                error = ERROR_NULL;
            }
        }
        else
        {
            error = ERROR_UNEXPECTED_VALUE;
        }
    }
    else
    {
        error = ERROR_OUT_OF_RANGE;
    }

    if(error != ERROR_NONE)
    {
        Error.printError(error, FILE_CONV);
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
 //must be a 3 bit value, returns a value between 0 and 8
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
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //must be between 0 and 8 value, returns a value between 0 and 8
 int OctConv::convToString(unsigned int rInReg, char* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

    for(i = 0; i < OCT_MAX; ++i)
    {
        if(rInReg == m_iOctTable[i])
        {
            bFound = true;
        } //else not this row, keep looking
    }

    if(bFound)
    {
        if(rOutReg)
        {
            *rOutReg = m_sOctTable[i];
        }
        else
        {
            error = ERROR_NULL;
        }
    }
    else
    {
        error = ERROR_UNEXPECTED_VALUE;
    }

    if(error != ERROR_NONE)
    {
        Error.printError(error, FILE_CONV);
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
 //must be betwee 0 and 8 value, returns a value between 0 and 8
 char OctConv::convToString(unsigned int rInReg)
 {
     int i = 0;
     bool bFound = false;
     char rOutReg = '0';

     for(i = 0; i < OCT_MAX; ++i)
     {
         if(rInReg == m_iOctTable[i])
         {
             bFound = true;
         } //else not this row, keep looking
     }

     if(bFound)
     {
         if(rOutReg)
         {
             rOutReg = m_sOctTable[i];
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
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //must be a 3 bit value, returns a value between 0 and 8
 int OctConv::convToNumber(bool* rInReg, unsigned int* rOutReg)
 {
    int error = ERROR_NONE;
    int iLength = getLength(rInReg);
    int i = 0;
    int col = 0;
    bool bFound = false;

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
            if(rOutReg)
            {
                *rOutReg = m_iOctTable[i];
            }
            else
            {
                error = ERROR_NULL;
            }
        }
        else
        {
            error = ERROR_UNEXPECTED_VALUE;
        }
    }
    else
    {
        error = ERROR_OUT_OF_RANGE;
    }

    if(error != ERROR_NONE)
    {
        Error.printError(error, FILE_CONV);
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
 //must be a 3 bit value, returns a value between 0 and 8
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
             if(rOutReg)
             {
                 rOutReg = m_iOctTable[i];
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
     }
     else
     {
         Error.printError(ERROR_OUT_OF_RANGE, FILE_CONV);
     }

     return rOutReg;
 }


 //================================================================================== 
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //must be between 0 and 8, returns a value between 0 and 8
 int OctConv::convToNumber(char rInReg, unsigned int* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

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
            *rOutReg = m_iOctTable[i];
        }
        else
        {
            error = ERROR_NULL;
        }
    }
    else
    {
        error = ERROR_UNEXPECTED_VALUE;
    }

    if(error != ERROR_NONE)
    {
        Error.printError(error, FILE_CONV);
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
 //must be between 0 and 8, returns a value between 0 and 8
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
             rOutReg = m_iOctTable[i];
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
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //must be between 0 and 8, returns a 3 bit value
 int OctConv::convToBinary(char rInReg, bool* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

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
            for(int j = 0; j < OCT_3BIT; ++j)
            {
                if(rOutReg[j])
                {
                    rOutReg[j] = m_bOctTable[i][j];
                }
                else
                {
                    error = ERROR_OUT_OF_RANGE;
                    break;
                }
            }
        }
        else
        {
            error = ERROR_NULL;
        }
    }
    else
    {
        error = ERROR_UNEXPECTED_VALUE;
    }

    if(error != ERROR_NONE)
    {
        Error.printError(error, FILE_CONV);
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
 //must be between 0 and 8, returns a 3 bit value
 bool* OctConv::convToBinary(char rInReg)
 {
     int i = 0;
     bool bFound = false;
     bool* rOutReg = new bool[OCT_MAX];

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
             for(int j = 0; j < OCT_3BIT; ++j)
             {
                 if(rOutReg[j])
                 {
                     rOutReg[j] = m_bOctTable[i][j];
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
 //Name:
 //Description:
 //Inputs:
 //Outputs:
 //Return:
 //================================================================================== 
 //must be less than 8, returns a 3 bit value
 int OctConv::convToBinary(unsigned int rInReg, bool* rOutReg)
 {
    int error = ERROR_NONE;
    int i = 0;
    bool bFound = false;

    for(i = 0; i < OCT_MAX; ++i)
    {
        if(rInReg == m_iOctTable[i])
        {
            bFound = true;
        } //else not this row, keep looking
    }

    if(bFound)
    {
        if(rOutReg)
        {
            for(int j = 0; j < OCT_3BIT; ++j)
            {
                if(rOutReg[j])
                {
                    rOutReg[j] = m_bOctTable[i][j];
                }
                else
                {
                    error = ERROR_NULL;
                    break;
                }
            }
        }
        else
        {
            error = ERROR_NULL;
        }
    }
    else
    {
        error = ERROR_UNEXPECTED_VALUE;
    }

    if(error != ERROR_NONE)
    {
        Error.printError(error, FILE_CONV);
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
 //must be less than 8, returns a 3 bit value
 bool* OctConv::convToBinary(unsigned int rInReg)
 {
     int i = 0;
     bool bFound = false;
     bool* rOutReg = new bool[OCT_MAX];

     for(i = 0; i < OCT_MAX; ++i)
     {
         if(rInReg == m_iOctTable[i])
         {
             bFound = true;
         } //else not this row, keep looking
     }

     if(bFound)
     {
         if(rOutReg)
         {
             for(int j = 0; j < OCT_3BIT; ++j)
             {
                 if(rOutReg[j])
                 {
                     rOutReg[j] = m_bOctTable[i][j];
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

