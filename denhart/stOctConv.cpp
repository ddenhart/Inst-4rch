#include "stOctConv.h"
#include "Common.h"
#include <cstdio>


//constructor
stOctConv::stOctConv()
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


//deconstructor
stOctConv::~stOctConv()
{

}


//returns the length of the boolean register
int stOctConv::getLength(bool* rInReg)
{
    int count = 0;

    while(rInReg)
    {
        ++count;
        ++rInReg;
    }

    return count;
}


//must be a 3 bit value, returns a value between 0 and 8
 int stOctConv::convToString(bool* rInReg, char* rOutReg)
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
                DEBUG_ERROR("OctConv: output structure is NULL");
                error = ERROR_OCTCONV_NULL;
            }
        }
        else
        {
            DEBUG_ERROR("OctConv: Couldn't find integer value");
            error = ERROR_OCTCONV_OPP_FAILED;
        }
    }
    else
    {
        DEBUG_ERROR("OctConv: Wrong input hex length");
        error = ERROR_OCTCONV_OPP_FAILED;
    }

    return error;
}

 //must be a 3 bit value, returns a value between 0 and 8
 char stOctConv::convToString(bool* rInReg)
 {
     int iLength = getLength(rInReg);
     int i = 0;
     int col = 0;
     bool bFound = false;
     char rOutReg = '0';

     if(iLength == OCT_3BIT)
     {
         for(i = 0; i < OCT_MAX; ++i)
         {
             if(rInReg[0] == m_bOctTable[i][col++])
             {
                 if(rInReg[1] == m_bOctTable[i][col++])
                 {
                     if(rInReg[2] == m_bOctTable[i][col])
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
             }
             else//else not this row, keep looking
             {
                 col = 0;
             } 
         }

         if(bFound)
         {
             if(rOutReg)
             {
                rOutReg = m_sOctTable[i];
             }
             else
             {
                 DEBUG_ERROR("OctConv: output structure is NULL");
             }
         }
         else
         {
             DEBUG_ERROR("OctConv: Couldn't find integer value");
         }
     }
     else
     {
         DEBUG_ERROR("OctConv: Wrong input hex length");
     }

     return rOutReg;
 }


 //must be between 0 and 8 value, returns a value between 0 and 8
 int stOctConv::convToString(unsigned int rInReg, char* rOutReg)
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
            DEBUG_ERROR("OctConv: output structure is NULL");
            error = ERROR_OCTCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("OctConv: Couldn't find integer value");
        error = ERROR_OCTCONV_OPP_FAILED;
    }

    return error;
 }


 //must be betwee 0 and 8 value, returns a value between 0 and 8
 char stOctConv::convToString(unsigned int rInReg)
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
             DEBUG_ERROR("OctConv: output structure is NULL");
         }
     }
     else
     {
         DEBUG_ERROR("OctConv: Couldn't find integer value");
     }

     return rOutReg;

 }


 //must be a 3 bit value, returns a value between 0 and 8
 int stOctConv::convToNumber(bool* rInReg, unsigned int* rOutReg)
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
                DEBUG_ERROR("OctConv: output structure is NULL");
                error = ERROR_OCTCONV_NULL;
            }
        }
        else
        {
            DEBUG_ERROR("OctConv: Couldn't find boolean value");
            error = ERROR_OCTCONV_OPP_FAILED;
        }
    }
    else
    {
        DEBUG_ERROR("OctConv: Wrong input hex length");
        error = ERROR_OCTCONV_OPP_FAILED;
    }

    return error;
 }


 //must be a 3 bit value, returns a value between 0 and 8
 unsigned int stOctConv::convToNumber(bool* rInReg)
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
                 DEBUG_ERROR("OctConv: output structure is NULL");
             }
         }
         else
         {
             DEBUG_ERROR("OctConv: Couldn't find boolean value");
         }
     }
     else
     {
         DEBUG_ERROR("OctConv: Wrong input hex length");
     }

     return rOutReg;
 }


 //must be between 0 and 8, returns a value between 0 and 8
 int stOctConv::convToNumber(char rInReg, unsigned int* rOutReg)
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
            DEBUG_ERROR("OctConv: output structure is NULL");
            error = ERROR_OCTCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("OctConv: Couldn't find character value");
        error = ERROR_OCTCONV_OPP_FAILED;
    }

    return error;
 }

 //must be between 0 and 8, returns a value between 0 and 8
 unsigned int stOctConv::convToNumber(char rInReg)
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
             DEBUG_ERROR("OctConv: output structure is NULL");
         }
     }
     else
     {
         DEBUG_ERROR("OctConv: Couldn't find character value");
     }

     return rOutReg;
 }


 //must be between 0 and 8, returns a 3 bit value
 int stOctConv::convToBinary(char rInReg, bool* rOutReg)
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
                    DEBUG_ERROR("OctConv: boolean array out of bounds");
                    error = ERROR_OCTCONV_OPP_FAILED;
                    break;
                }
            }
        }
        else
        {
            DEBUG_ERROR("OctConv: output structure is NULL");
            error = ERROR_OCTCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("OctConv: Couldn't find character value");
        error = ERROR_OCTCONV_OPP_FAILED;
    }

    return error;
 }


 //must be between 0 and 8, returns a 3 bit value
 bool* stOctConv::convToBinary(char rInReg)
 {
     int i = 0;
     bool bFound = false;
     bool rOutReg[OCT_MAX];

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
                     DEBUG_ERROR("OctConv: boolean array out of bounds");
                     break;
                 }
             }
         }
         else
         {
             DEBUG_ERROR("OctConv: output structure is NULL");
         }
     }
     else
     {
         DEBUG_ERROR("OctConv: Couldn't find character value");
     }

     return rOutReg;
 }


 //must be less than 8, returns a 3 bit value
 int stOctConv::convToBinary(unsigned int rInReg, bool* rOutReg)
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
                    DEBUG_ERROR("OctConv: boolean array out of bounds");
                    error = ERROR_OCTCONV_OPP_FAILED;
                    break;
                }
            }
        }
        else
        {
            DEBUG_ERROR("OctConv: output structure is NULL");
            error = ERROR_OCTCONV_NULL;
        }
    }
    else
    {
        DEBUG_ERROR("OctConv: Couldn't find integer value");
        error = ERROR_OCTCONV_OPP_FAILED;
    }

    return error;
 }


 //must be less than 8, returns a 3 bit value
 bool* stOctConv::convToBinary(unsigned int rInReg)
 {
     int i = 0;
     bool bFound = false;
     bool rOutReg[OCT_MAX];

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
                     DEBUG_ERROR("OctConv: boolean array out of bounds");
                     break;
                 }
             }
         }
         else
         {
             DEBUG_ERROR("OctConv: output structure is NULL");
         }
     }
     else
     {
         DEBUG_ERROR("OctConv: Couldn't find integer value");
     }

     return rOutReg;
 }



