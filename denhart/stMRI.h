//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "st7bitReg.h"



class stMRI
{

private:
    
    bool r1bitInderection;
    bool r1bitMemoryPage;
    st7bitReg r7bitOffs;

public:

    // Memory Page (0:Zero Page/1:Current Page)
    int handleM();
    void calcEffectiveAddy();
    /// Indirect Addressing Bit (0:Direct/1:Indirect)
    int handleI();
    void handleAND();
    void handleTAD();
    void handleJMS();
    void handleDCA();
    void handleISZ();
    void handleJMP();

};

