#include <string.h>

#include <localUtil.h>
#include <hde.h>

#include "../opcOperand.h"
#include "opcOp_arch.h"

cOperand_amd64::cOperand_amd64()
{
    fixvar_set = (e_imm | e_disp);
    memset(&parsedOpcode, 0, sizeof(hde64s));
}

int cOperand_amd64::initme(uint8_t* initdata)
{
    return hde64_disasm(initdata, &parsedOpcode);
}

bool cOperand_amd64::checkHelper(cOperand* targCompare)
{
    bool result = false;
    cOperand_amd64* targCompare_l = (cOperand_amd64*)targCompare;

    SAFE_BAIL(parsedOpcode.opcode != targCompare_l->parsedOpcode.opcode);
    SAFE_BAIL(parsedOpcode.len != targCompare_l->parsedOpcode.len);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, rex_w);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, rex_r);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, rex_x);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, rex_b);
    CMPASSIGN_REG_IND(parsedOpcode, targCompare_l, imm, imm.imm64);
    CMPASSIGN_REG_IND(parsedOpcode, targCompare_l, disp, disp.disp32);

    result = true;
fail:
    return result;
}

int cOperand_amd64::getOpComp(val_set_t val_set, size_t* component)
{
#define EACH_CASE_IND(INDEX, STRUCT_INDEX) \
    case e_ ## INDEX: \
        *component = parsedOpcode.STRUCT_INDEX; \
        break;

#define EACH_CASE(INDEX) \
    EACH_CASE_IND(INDEX, INDEX)

    int result = -1;

    switch (val_set)
    {
    EACH_CASE_IND(imm, imm.imm64);
    EACH_CASE_IND(disp, disp.disp32);
    default:
        goto fail;
    }
    result = 0;
fail:
    return result;
}
