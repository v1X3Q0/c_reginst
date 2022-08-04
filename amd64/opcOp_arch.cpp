#include <string.h>

#include <localUtil.h>
#include <hde.h>

#include "../opcOperand.h"
#include "opcOp_arch.h"

cOperand_amd64::cOperand_amd64()
{
    fixvar_set = (
        e_reg_src1 | e_reg_src2 | e_reg_dst1 | e_reg_dst2 | 
        e_imm | e_disp);
    memset(&parsedOpcode, 0, sizeof(hde64s_t));
}

int cOperand_amd64::initme(uint8_t* initdata)
{
    return parseInst(initdata, &parsedOpcode);
}

size_t cOperand_amd64::getinstsz()
{
    return parsedOpcode.len;
}

bool cOperand_amd64::checkHelper(cOperand* targCompare)
{
    bool result = false;
    cOperand_amd64* targCompare_l = (cOperand_amd64*)targCompare;

    SAFE_BAIL(parsedOpcode.opcode1 != targCompare_l->parsedOpcode.opcode1);
    // like i mentioned before, cause of the potential var len for instructions
    // SAFE_BAIL(parsedOpcode.len != targCompare_l->parsedOpcode.len);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, len);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, reg_src1);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, reg_src2);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, reg_dst1);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, reg_dst2);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, addrmode);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, imm);
    CMPASSIGN_REG(parsedOpcode, targCompare_l, disp);

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
        // lastly, need this for var len
    EACH_CASE(len);
    EACH_CASE(reg_src1);
    EACH_CASE(reg_src2);
    EACH_CASE(reg_dst1);
    EACH_CASE(reg_dst2);
    // these definitions consistent with our custom struct
    EACH_CASE(imm);
    EACH_CASE(disp);
    default:
        goto fail;
    }
    result = 0;
fail:
    return result;
}
