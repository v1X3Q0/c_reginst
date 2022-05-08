#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <localUtil.h>

#ifdef __arm64__
#include <hdeA64.h>
#else
#include <capstone/capstone.h>
#endif

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

// case for adding a variable size_t, inwhich we just are adding a * to a **
// template<typename T>
void cOperand::fixvar_add(size_t* targetVar, saveVar_t var_member, val_set_t e_index)
{
    var_member->regRand = this;
    var_member->val_set = e_index;
    fixvar_set &= ~e_index;
    fixvar_unmod = fixvar_set;
}

