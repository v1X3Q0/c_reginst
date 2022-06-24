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

    size_t getinstsz();
    bool checkHelper(cOperand* targCompare);
    int getOpComp(val_set_t val_set, size_t* component);

    template <
        typename fv_reg_src1, typename fv_reg_src2, typename fv_reg_dst1, typename fv_reg_dst2,
        typename fv_imm, typename fv_disp>
    static cOperand* insertToGlob(uint64_t opcode, uint8_t len, uint8_t addrmode,
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
        FIXVAR_ADD(addrmode, outOp);
        FIXVAR_ADD(disp, outOp);
        FIXVAR_ADD(imm, outOp);
        return outOp;
    }

    // mov rax, 0xfffffa8000000008
    template <typename fv_reg_dst1, typename fv_imm>
    static cOperand* createMOVRI(fv_reg_dst1 reg_dst1, fv_imm imm)
    {
        return insertToGlob<
            size_t, size_t, fv_reg_dst1, size_t,
            fv_imm, size_t>(MOV_RI, 0x0a, NULL,
                0, 0, reg_dst1, 0,
                imm, 0);
    }
    
    // mov rax, [relative_addr]
    template <typename fv_reg_dst1, typename fv_disp>
    static cOperand* createMOVRREL(fv_reg_dst1 reg_dst1, fv_disp disp)
    {
        return insertToGlob<
            size_t, size_t, fv_reg_dst1, size_t,
            size_t, fv_disp>(MOV_RR, 0x07, ADDRMODE_REL,
                0x05, 0, reg_dst1, 0,
                0, disp);
    }

    // mov rax, rax
    template <typename fv_reg_dst1, typename fv_reg_src1>
    static cOperand* createMOVRR(fv_reg_dst1 reg_dst1, fv_reg_src1 reg_src1)
    {
        return insertToGlob<
            fv_reg_src1, size_t, fv_reg_dst1, size_t,
            size_t, size_t>(MOV_RR, 0x03, ADDRMODE_RR,
                reg_src1, 0, reg_dst1, 0,
                0, 0);
    }

    // mov rax, [rax + 0x08]
    // for when the offset is a byte
    template <typename fv_reg_dst1, typename fv_reg_src1, typename fv_disp>
    static cOperand* createMOVRRI(fv_reg_dst1 reg_dst1, fv_reg_src1 reg_src1, fv_disp disp)
    {
        return insertToGlob<
            fv_reg_src1, size_t, fv_reg_dst1, size_t,
            size_t, fv_disp>(MOV_RR, 0x05, ADDRMODE_DREF,
                reg_src1, 0, reg_dst1, 0,
                0, disp);
    }

    // mov rax, [rax + 0x0650]
    // for when the offset is a word
    template <typename fv_reg_dst1, typename fv_reg_src1, typename fv_disp>
    static cOperand* createMOVRRwI(fv_reg_dst1 reg_dst1, fv_reg_src1 reg_src1, fv_disp disp)
    {
        return insertToGlob<
            fv_reg_src1, size_t, fv_reg_dst1, size_t,
            size_t, fv_disp>(MOV_RR, 0x07, ADDRMODE_DREFW,
                reg_src1, 0, reg_dst1, 0,
                0, disp);
    }

    // call rel_addr
    template <typename fv_imm>
    static cOperand* createCALLREL(fv_imm imm)
    {
        return insertToGlob<
            size_t, size_t, size_t, size_t,
            fv_imm, size_t>(CALL_I, 0x05, NULL,
                0, 0, 0, 0,
                imm, 0);
    }

    // mov [rel_addr], rax
    // funny comment for this one, src reg has to be 0x05, and dest is actually
    // the source register. Just works that way
    template <typename fv_disp, typename fv_reg_src1>
    static cOperand* createMOVRADDR_R(fv_disp disp, fv_reg_src1 reg_src1)
    {
        return insertToGlob<
            size_t, size_t, fv_reg_src1, size_t,
            size_t, fv_disp>(MOV_RADDR_R, 0x07, ADDRMODE_REL,
                0x05, 0, reg_src1, 0, 
                0, disp);
    }

    // call [rel_addr]
    template <typename fv_disp>
    static cOperand* createCALLDREFREL(fv_disp disp)
    {
        return insertToGlob<
            size_t, size_t, size_t, size_t,
            size_t, fv_disp>(CALL_DREFREL, 0x06, NULL,
                0x05, 0, 0x02, 0,
                0, disp);
    }

};

#endif