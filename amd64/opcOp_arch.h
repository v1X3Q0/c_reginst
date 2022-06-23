#ifndef OPCOP_AARCH64_H
#define OPCOP_AARCH64_H

#include <hde.h>
#include "../opcOperand.h"


class cOperand_amd64 : public cOperand
{
protected:

public:
    hde64s_t parsedOpcode;
    // constructor for a standard operator
    // initiate it to an initialized fixed variable, that's it.
    cOperand_amd64();

    int initme(uint8_t* initdata);

    size_t getinstsz() { return parsedOpcode.len; };

    bool checkHelper(cOperand* targCompare);
    int getOpComp(val_set_t val_set, size_t* component);

    template <
        typename fv_reg_src1, typename fv_reg_src2, typename fv_reg_dst1, typename fv_reg_dst2,
        typename fv_imm, typename fv_disp>
    static cOperand* insertToGlob(uint64_t opcode, uint8_t len,
        fv_reg_src1 reg_src1, fv_reg_src2 reg_src2, fv_reg_dst1 reg_dst1, fv_reg_dst2 reg_dst2,
        fv_imm imm, fv_disp disp)
    {
        cOperand_amd64* outOp = new cOperand_amd64();
        outOp->parsedOpcode.opcode1 = opcode;
        outOp->parsedOpcode.len = len;
        FIXVAR_ADD(reg_src1, outOp);
        FIXVAR_ADD(reg_src2, outOp);
        FIXVAR_ADD(reg_dst1, outOp);
        FIXVAR_ADD(reg_dst2, outOp);
        // FIXVAR_ADD(disp, outOp);
        outOp->fixvar_add(&outOp->parsedOpcode.disp, disp, e_disp);
        FIXVAR_ADD(imm, outOp);
        return outOp;
    }

    // mov rax, 0xfffffa8000000008
    template <typename fv_reg_dst1, typename fv_imm>
    static cOperand* createMOVRI(fv_reg_dst1 reg_dst1, fv_imm imm)
    {
        return insertToGlob<
            size_t, size_t, fv_reg_dst1, size_t,
            fv_imm, size_t>(MOV_RI, 0x0a,
                0, 0, reg_dst1, 0,
                imm, 0);
    }

    // mov rax, rax
    template <typename fv_reg_dst1, typename fv_reg_src1>
    static cOperand* createMOVRR(fv_reg_dst1 reg_dst1, fv_reg_src1 reg_src1)
    {
        return insertToGlob<
            fv_reg_src1, size_t, fv_reg_dst1, size_t,
            size_t, size_t>(MOV_RR, 0x03,
                reg_src1, 0, reg_dst1, 0,
                0, 0);
    }

    // mov rax, [rax + 0x08]
    template <typename fv_reg_dst1, typename fv_reg_src1, typename fv_imm>
    static cOperand* createMOVRRI(fv_reg_dst1 reg_dst1, fv_reg_src1 reg_src1, fv_imm imm)
    {
        return insertToGlob<
            fv_reg_src1, size_t, fv_reg_dst1, size_t,
            fv_imm, size_t>(MOV_RR, 0x05,
                reg_src1, 0, reg_dst1, 0,
                imm, 0);
    }

    template <typename fv_imm>
    static cOperand* createCALLREL(fv_imm imm)
    {
        return insertToGlob<
            size_t, size_t, size_t, size_t,
            fv_imm, size_t>(CALL_I, 0x05,
                0, 0, 0, 0,
                imm, 0);
    }

    // template <typename fv_rex_w, typename fv_imm>
    // static cOperand* createMOVRADDR_R(fv_rex_w rex_w, fv_imm imm)
    // {
    //     return insertToGlob<
    //         fv_rex_w, size_t, size_t, size_t,
    //         fv_imm, size_t>(MOV_RADDR_R, 0x05,
    //             rex_w, 0, 0, 0, 
    //             imm, 0);
    // }

};

#endif