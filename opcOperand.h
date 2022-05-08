#ifndef OPCOPERAND_H
#define OPCOPERAND_H

#include <stdint.h>

#ifdef __arm64__
#include <hdeA64.h>
#else
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
public:
    hde_t parsedOpcode;

    template <typename T>
    static cOperand* createOp(uint8_t* curAddr)
    {
        T* cOptmp = new T;
        cOptmp->initme(curAddr);
        return cOptmp;
    }

    virtual int initme(uint8_t* initdata) = 0;

    // routine for child to compare against a register
    virtual bool checkHelper(cOperand* targCompare) = 0;
    // accessor, for when getting variables need to operate on results
    virtual int getOpComp(val_set_t val_set, size_t* component) = 0;

    virtual size_t getinstsz() = 0;
    // case for adding a fixed size_t 
    // template <typename T>
    void fixvar_add(size_t* targetVar, size_t hde_member, val_set_t e_index);

    // case for adding a variable size_t, inwhich we just are adding a * to a **
    // template<typename T>
    void fixvar_add(size_t* targetVar, saveVar_t var_member, val_set_t e_index);

    void clearVars();
};

#endif