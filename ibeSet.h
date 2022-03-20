#ifndef IBESET_H
#define IBESET_H

#include <stdarg.h>
#include <list>
#include <map>

#include <localUtil.h>

#include <bgrep_e.h>
#include "opcOperand.h"

#ifdef LIVE_KERNEL
#include "hdeA64.h"
#else
#include <capstone/capstone.h>
#endif

class instSet : public search_set
{
private:
    std::map<uint32_t, saveVar_t> varTable;
    std::list<cOperand*> instPatternList;
public:
    instSet() : search_set(0, 1, true) {};
    void clearInternals();
    saveVar_t checkOperand(uint32_t newOperand);
    saveVar_t addOperand(uint32_t newOperand, val_set_t val_set, cOperand* regRand);
    void addNewInst(cOperand* newInstruction);
    int getVar(uint32_t key, size_t* value);
    void clearVars();

    // base class requirement
    int findPattern(uint8_t* startAddress_a, size_t sizeSearch, void** resultAddr_a);
    // opcOperandVar* useKey(uint32_t key);
};

#endif