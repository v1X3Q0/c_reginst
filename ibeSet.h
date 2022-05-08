#ifndef IBESET_H
#define IBESET_H

#include <stdarg.h>
#include <list>
#include <map>

#include <localUtil.h>

#include <bgrep_e.h>
#include "opcOperand.h"

typedef enum
{
    AARCH64_IBE=1,
    X86_64_IBE,
    I386_IBE,
    ARM_IBE
} ARCH_IBE_t;

class instSet : public search_set
{
private:
    std::map<uint32_t, saveVar_t> varTable;
    std::list<cOperand*> instPatternList;
    ARCH_IBE_t ibe_arch;

    int incrementInst(uint8_t* curAddr, cOperand* curInst, uint8_t** resultAddr);

public:
    instSet(ARCH_IBE_t ibe_arch_a) : search_set(0, 1, true), ibe_arch(ibe_arch_a) {};
    instSet(ARCH_IBE_t ibe_arch_a, size_t offset_a, size_t step_a) : search_set(offset_a, step_a, true), ibe_arch(ibe_arch_a) {};
    void clearInternals();
    saveVar_t checkOperand(uint32_t newOperand);
    saveVar_t addOperand(uint32_t newOperand, val_set_t val_set, cOperand* regRand);
    void addNewInst(cOperand* newInstruction);
    int getVar(uint32_t key, size_t* value);
    void clearVars();

    // base class requirement
    int findPattern_fixed(uint8_t* startAddress_a, size_t sizeSearch, void** resultAddr_a);
    // opcOperandVar* useKey(uint32_t key);
};

#endif