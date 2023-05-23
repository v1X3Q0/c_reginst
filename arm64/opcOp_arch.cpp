#include <string.h>

#include <localUtil.h>
#include <hdeA64.h>

#include "opcOp_arch.h"

cOperand_arm64::cOperand_arm64()
{
    fixvar_set = (e_rd | e_rn | e_rm | e_imms | e_immr | e_immLarge);
    memset(&parsedOpcode, 0, sizeof(hdeA64_t));
}

int cOperand_arm64::initme(uint8_t* initdata)
{
    return parseByEnc(*(uint32_t*)initdata, &parsedOpcode);
}

bool cOperand_arm64::checkHelper(cOperand* targCompare_a)
{
    bool result = false;
    cOperand_arm64* targCompare = (cOperand_arm64*)targCompare_a;

    SAFE_BAIL(parsedOpcode.opcode != targCompare->parsedOpcode.opcode);
    // CMPASSIGN_REG(parsedOpcode, targCompare, rd);
    if (fixvar_set & e_rd)
    {
        if (parsedOpcode.rd != targCompare->parsedOpcode.rd)
        {
            goto fail;
        };
    }
    else
    {
        parsedOpcode.rd = targCompare->parsedOpcode.rd;
        fixvar_set ^= e_rd;
    }
    if (fixvar_set & e_rn)
    {
        if (parsedOpcode.rn != targCompare->parsedOpcode.rn)
        {
            goto fail;
        };
    }
    else
    {
        parsedOpcode.rn = targCompare->parsedOpcode.rn;
        fixvar_set ^= e_rn;
    }
    CMPASSIGN_REG(parsedOpcode, targCompare, rm);
    CMPASSIGN_REG(parsedOpcode, targCompare, imms);
    if (fixvar_set & e_immr)
    {
        if (parsedOpcode.immr != targCompare->parsedOpcode.immr)
        {
            goto fail;
        };
    }
    else
    {
        parsedOpcode.immr = targCompare->parsedOpcode.immr;
        fixvar_set ^= e_immr;
    }
    if (fixvar_set & e_immLarge)
    {
        if (parsedOpcode.immLarge != targCompare->parsedOpcode.immLarge)
        {
            goto fail;
        };
    }
    else
    {
        parsedOpcode.immLarge = targCompare->parsedOpcode.immLarge;
        fixvar_set ^= e_immLarge;
    }

    result = true;
fail:
    return result;
}

int cOperand_arm64::getOpComp(val_set_A64_t val_set, size_t* component)
{
#define EACH_CASE(INDEX) \
    case e_ ## INDEX: \
        *component = parsedOpcode.INDEX; \
        break;

    int result = -1;

    switch (val_set)
    {
    // EACH_CASE(rd);
    case e_rd:
        *component = parsedOpcode.rd;
        break;
    EACH_CASE(rn);
    EACH_CASE(rm);
    EACH_CASE(imms);
    EACH_CASE(immr);
    EACH_CASE(immLarge);
    default:
        goto fail;
    }
    result = 0;
fail:
    return result;
}
