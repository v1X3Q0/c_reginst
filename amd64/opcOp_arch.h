#ifndef OPCOP_AARCH64_H
#define OPCOP_AARCH64_H

#include <hde.h>
#include "../opcOperand.h"

class cOperand_amd64 : public cOperand
{
protected:
    
public:
    // constructor for a standard operator
    // initiate it to an initialized fixed variable, that's it.
    cOperand_amd64();

    int initme(uint8_t* initdata);

    size_t getinstsz() { return sizeof(uint32_t); };

    bool checkHelper(cOperand* targCompare);
    int getOpComp(val_set_t val_set, size_t* component);

    template <typename fv_rd, typename fv_rn, typename fv_rm, typename fv_imms, typename fv_immr, typename fv_immLarge>
    static cOperand* insertToGlob(uint64_t opcode, fv_rd rd, fv_rn rn, fv_rm rm, fv_imms imms, fv_immr immr, fv_immLarge immLarge)
    {
        cOperand* outOp = new cOperand_amd64();
        outOp->parsedOpcode.opcode = opcode;
        FIXVAR_ADD(rd, outOp);
        FIXVAR_ADD(rn, outOp);
        FIXVAR_ADD(rm, outOp);
        FIXVAR_ADD(imms, outOp);
        FIXVAR_ADD(immr, outOp);
        FIXVAR_ADD(immLarge, outOp);
        return outOp;
    }

};

#endif