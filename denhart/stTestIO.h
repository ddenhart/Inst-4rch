//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st3bitReg.h"


#define ID_LENG 6


class stTestIO
{

private:

    bool bDeviceNumber[ID_LENG];
    st3bitReg bFCN;
    bool bNum3;
    bool bNum11;

public:

    void group1();
    void group3();
    void group2();
    void determineGroup();
    void checkTwoInst();
    void checkIllegalCombo();

};

