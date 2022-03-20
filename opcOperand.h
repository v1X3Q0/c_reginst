#ifndef OPCOPERAND_H
#define OPCOPERAND_H

#include <stdint.h>

#ifdef LIVE_KERNEL
#include <hdeA64.h>
#else
#include <capstone/capstone.h>
#include "hde_placehold.h"
#endif
// #include <ibeSet.h>

#define CMPASSIGN_REG(LOCAL, TARGET, INDEX) \
    if (fixvar_set & e_ ## INDEX) \
    { \
        SAFE_BAIL(LOCAL.INDEX != TARGET->LOCAL.INDEX); \
    } \
    else \
    { \
        LOCAL.INDEX = TARGET->LOCAL.INDEX; \
        fixvar_set ^= e_  ## INDEX; \
    }

#define FIXVAR_ADD(INDEX, OPERAND) \
    OPERAND->fixvar_add(&OPERAND->parsedOpcode.INDEX, INDEX, e_ ## INDEX);
    // fixvar_add<typeof(hde_t::INDEX)>(&OPERAND->parsedOpcode.INDEX, INDEX, e_ ## INDEX);

class cOperand;

typedef struct
{
    val_set_t val_set;
    cOperand* regRand;
} saveVar, *saveVar_t;

class cOperand
{
protected:
// if the variable has been fixed
    uint32_t fixvar_unmod;
    uint32_t fixvar_set;
    hde_t parsedOpcode;
public:
// initiate it to an initialized fixed variable, that's it.
    cOperand();
// constructor for fixed operands
    cOperand(uint32_t targOp);
    bool checkHelper(cOperand* targCompare);

    int getOpComp(val_set_t val_set, size_t* component);

    // case for adding a fixed size_t 
    // template <typename T>
    void fixvar_add(size_t* targetVar, size_t hde_member, val_set_t e_index);

    // case for adding a variable size_t, inwhich we just are adding a * to a **
    // template<typename T>
    void fixvar_add(size_t* targetVar, saveVar_t var_member, val_set_t e_index);

    void clearVars();
};

#endif