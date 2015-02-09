
//------------------------------------------------------------------------------
#include "st12bitReg.h"
#include "st3bitReg.h"
#include "stMemNode.h"


class stMemList
{

private:

    /// points to the head of the list
    stMemNode *head;
    // points to the latest node in the list
    stMemNode *current;
    // used to traverse the list
    stMemNode *temp;

public:

    // sets temp to th next pointer of the current temp
    int setTempToNext();
    // retrieves the type and addy stored in the node address of temp
    int getMemData(st3bitReg *iType, st12bitReg *iAddy);
    // saves the type and addy provided to the end of the list
    int setMemData(st3bitReg iType, st12bitReg iAddy);
    // set next and current to the newest node
    void setNext(stMemNode *next);
    // deletes all nodes in the list
    void clearList();
    /// sets temp to the begining of the list
    int setTempToHead();

};

