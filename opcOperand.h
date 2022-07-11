#ifndef OPCOPERAND_H
#define OPCOPERAND_H

#include <stdint.h>

#if defined(SUPPORT_X86_64)
#include <hde.h>
#elif defined(SUPPORT_AARCH64)
#include <hdeA64.h>
#endif
// #include <ibeSet.h>

#define CMPASSIGN_REG_IND(LOCAL, TARGET, INDEX, STRUCT_INDEX) \
    if (fixvar_set & e_ ## INDEX) \
    { \
        SAFE_BAIL(LOCAL.STRUCT_INDEX != TARGET->LOCAL.STRUCT_INDEX); \
    } \
    else \
    { \
        LOCAL.STRUCT_INDEX = TARGET->LOCAL.STRUCT_INDEX; \
        fixvar_set ^= e_  ## INDEX; \
    }

#define CMPASSIGN_REG(LOCAL, TARGET, INDEX) \
    CMPASSIGN_REG_IND(LOCAL, TARGET, INDEX, INDEX)

// #define FIXVAR_ADD(INDEX, OPERAND) \
//     OPERAND->fixvar_add(&OPERAND->parsedOpcode.INDEX, INDEX, e_ ## INDEX);
    // fixvar_add<typeof(hde_t::INDEX)>(&OPERAND->parsedOpcode.INDEX, INDEX, e_ ## INDEX);

#define FIXVAR_ADD_IND(INDEX, STRUCT_INDEX, OPERAND) \
    OPERAND->fixvar_add(&OPERAND->parsedOpcode.STRUCT_INDEX, INDEX, e_ ## INDEX);

#define FIXVAR_ADD(INDEX, OPERAND) \
    FIXVAR_ADD_IND(INDEX, INDEX, OPERAND)

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
    // hde_t parsedOpcode;

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
    void fixvar_add(uint8_t* targetVar, size_t hde_member, val_set_t e_index);
    void fixvar_add(uint16_t* targetVar, size_t hde_member, val_set_t e_index);
    void fixvar_add(uint32_t* targetVar, size_t hde_member, val_set_t e_index);
    void fixvar_add(int32_t* targetVar, size_t hde_member, val_set_t e_index);
    void fixvar_add(int64_t* targetVar, size_t hde_member, val_set_t e_index);
    void fixvar_add(long* targetVar, size_t hde_member, val_set_t e_index);

    // case for adding a variable size_t, inwhich we just are adding a * to a **
    // template<typename T>
    void fixvar_add(size_t* targetVar, saveVar_t var_member, val_set_t e_index);
    void fixvar_add(uint8_t* targetVar, saveVar_t var_member, val_set_t e_index);
    void fixvar_add(uint16_t* targetVar, saveVar_t var_member, val_set_t e_index);
    void fixvar_add(uint32_t* targetVar, saveVar_t var_member, val_set_t e_index);
    void fixvar_add(int32_t* targetVar, saveVar_t var_member, val_set_t e_index);
    void fixvar_add(int64_t* targetVar, saveVar_t var_member, val_set_t e_index);
    void fixvar_add(long* targetVar, saveVar_t var_member, val_set_t e_index);

    void clearVars();
};

#endif