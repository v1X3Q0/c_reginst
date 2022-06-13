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

    template <typename fv_rex_w, typename fv_rex_r, typename fv_rex_x,
        typename fv_rex_b, typename fv_imm, typename fv_disp>
    static cOperand* insertToGlob(uint64_t opcode, uint8_t len, fv_rex_w rex_w,
        fv_rex_r rex_r, fv_rex_x rex_x, fv_rex_b rex_b, fv_imm imm, fv_disp disp)
    {
        cOperand_amd64* outOp = new cOperand_amd64();
        outOp->parsedOpcode.opcode = opcode;
        outOp->parsedOpcode.len = len;
        FIXVAR_ADD(rex_w, outOp);
        FIXVAR_ADD(rex_r, outOp);
        FIXVAR_ADD(rex_x, outOp);
        FIXVAR_ADD(rex_b, outOp);
        FIXVAR_ADD_IND(imm, imm.imm64, outOp);
        FIXVAR_ADD_IND(disp, disp.disp32, outOp);
        return outOp;
    }

    // alias for ORR register, where it is mov reg, reg
    template <typename fv_rex_w, typename fv_imm>
    static cOperand* createMOVRI(fv_rex_w rex_w, fv_imm imm)
    {
        return insertToGlob<fv_rex_w, size_t, size_t, size_t, fv_imm, size_t>(MOV_RI, 0xa, rex_w, 0, 0, 0, imm, 0);
    }

};

#endif