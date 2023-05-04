#ifndef IBESET_H
#define IBESET_H

#include <stdarg.h>
#include <list>
#include <map>

#include <localUtil.h>

#include <bgrep_e.h>
#include "opcOperand.h"

#if defined(SUPPORT_X86_64)
#include "amd64/opcOp_arch.h"
#endif
#if defined(SUPPORT_AARCH64)
#include "arm64/opcOp_arch.h"
#endif

typedef enum
{
    AARCH64_IBE=1,
    X86_64_IBE,
    I386_IBE,
    ARM_IBE
} ARCH_IBE_t;

template <typename cOperand_t, typename val_set_t>
class instSet : public search_set
{
private:

    typedef saveVar<val_set_t> *saveVar_t;

    std::map<uint32_t, saveVar_t> varTable;
    std::list<cOperand<val_set_t>*> instPatternList;
    ARCH_IBE_t ibe_arch;

    int incrementInst(uint8_t* curAddr, cOperand<val_set_t>* curInst, uint8_t** resultAddr);

public:
    instSet(ARCH_IBE_t ibe_arch_a) : search_set(0, 1, true), ibe_arch(ibe_arch_a) {};
    instSet(ARCH_IBE_t ibe_arch_a, size_t offset_a, size_t step_a) : search_set(offset_a, step_a, true), ibe_arch(ibe_arch_a) {};

    instSet() : search_set(0, 1, true) {};
    instSet(size_t offset_a, size_t step_a) : search_set(offset_a, step_a, true) {};

    void clearInternals();
    saveVar_t checkOperand(uint32_t newOperand);
    void addNewInst(cOperand<val_set_t>* newInstruction);

    int getVar(uint32_t key, size_t *value);
    void clearVars();
    int getinstoff(uint32_t index, size_t* instOff);

    // deprecated
    // saveVar_t addOperand(uint32_t newOperand, val_set_t val_set, cOperand* regRand);

protected:
    // base class requirement
    int findPattern_fixed(uint8_t* startAddress_a, size_t sizeSearch, void** resultAddr_a);

    // deprecated
    // opcOperandVar* useKey(uint32_t key);
};

#include "ibeSet.hpp"

#endif