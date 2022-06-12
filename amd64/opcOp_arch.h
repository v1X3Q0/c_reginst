#ifndef OPCOP_AARCH64_H
#define OPCOP_AARCH64_H

#include <hde.h>
#include "../opcOperand.h"



class cOperand_amd64 : public cOperand
{
protected:

public:
    hde64s parsedOpcode;
    // constructor for a standard operator
    // initiate it to an initialized fixed variable, that's it.
    cOperand_amd64();

    int initme(uint8_t* initdata);

    size_t getinstsz() { return parsedOpcode.len; };

    bool checkHelper(cOperand* targCompare);
    int getOpComp(val_set_t val_set, size_t* component);

    template <typename fv_imm, typename fv_disp>
    static cOperand* insertToGlob(uint64_t opcode, uint8_t len, fv_imm imm, fv_disp disp)
    {
        cOperand_amd64* outOp = new cOperand_amd64();
        outOp->parsedOpcode.opcode = opcode;
        outOp->parsedOpcode.len = len;
        FIXVAR_ADD(imm, outOp);
        FIXVAR_ADD(disp, outOp);
        return outOp;
    }

};

#endif