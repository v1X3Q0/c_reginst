#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <localUtil.h>

#include "opcOperand.h"

void cOperand::clearVars()
{
    fixvar_set = fixvar_unmod;
}

// case for adding a fixed size_t 
// template <typename T>
void cOperand::fixvar_add(size_t* targetVar, size_t hde_member, val_set_t e_index)
{
    // register technically can't be higher than 0x1f, so do this to FE
    // mask -1 to be 0x1f
    if (e_index & VAL_SET_REGMASK)
    {
        hde_member &= 0x1f;
    }
    *targetVar = (size_t)hde_member;
    fixvar_set |= e_index;
    fixvar_unmod = fixvar_set;
}

#define FIXVAR_UINTX(uintx_t) \
    void cOperand::fixvar_add(uintx_t* targetVar, size_t hde_member, val_set_t e_index) \
    { \
        if (e_index & VAL_SET_REGMASK) \
        { \
            hde_member &= 0x1f; \
        } \
        *targetVar = (uintx_t)hde_member; \
        fixvar_set |= e_index; \
        fixvar_unmod = fixvar_set; \
    }

FIXVAR_UINTX(uint8_t);
FIXVAR_UINTX(uint32_t);
FIXVAR_UINTX(int32_t);
FIXVAR_UINTX(int64_t);
#ifdef __APPLE__
FIXVAR_UINTX(long);
#endif

// case for adding a variable size_t, inwhich we just are adding a * to a **
// template<typename T>
void cOperand::fixvar_add(size_t* targetVar, saveVar_t var_member, val_set_t e_index)
{
    var_member->regRand = this;
    var_member->val_set = e_index;
    fixvar_set &= ~e_index;
    fixvar_unmod = fixvar_set;
}

#define FIXVAR_SAVEVAR_UINTX(uintx) \
void cOperand::fixvar_add(uintx* targetVar, saveVar_t var_member, val_set_t e_index) \
{ \
    var_member->regRand = this; \
    var_member->val_set = e_index; \
    fixvar_set &= ~e_index; \
    fixvar_unmod = fixvar_set; \
}

FIXVAR_SAVEVAR_UINTX(uint8_t);
FIXVAR_SAVEVAR_UINTX(uint16_t);
FIXVAR_SAVEVAR_UINTX(uint32_t);
FIXVAR_SAVEVAR_UINTX(int32_t);
FIXVAR_SAVEVAR_UINTX(int64_t);
#ifdef __APPLE__
FIXVAR_SAVEVAR_UINTX(long);
#endif