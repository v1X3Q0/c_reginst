#ifndef OPCOP_AARCH64_H
#define OPCOP_AARCH64_H

#include <hdeA64.h>
#include "../opcOperand.h"

class cOperand_arm64 : public cOperand<val_set_A64_t>
{
protected:
    
public:

    typedef saveVar<val_set_A64_t> *saveVar_t;

    hdeA64_t parsedOpcode;
    // constructor for a standard operator
    // initiate it to an initialized fixed variable, that's it.
    cOperand_arm64();
    // cOperand_arm64(uint8_t* curaddr) : cOperand(curaddr) {};
    cOperand_arm64(uint8_t* curaddr)
    {
        initme(curaddr);
    };

    int initme(uint8_t* initdata);

    size_t getinstsz() { return sizeof(uint32_t); };

    bool checkHelper(cOperand* targCompare);
    int getOpComp(val_set_A64_t val_set, size_t* component);

    // this function will set the variable and non variable args. passed into it is an array of
    // fixed and variable args, so they get set in the internal. fixvar_set is the internal
    // tracker for if something is a fixed
    template <typename fv_rd, typename fv_rn, typename fv_rm, typename fv_imms, typename fv_immr, typename fv_immLarge>
    static cOperand* insertToGlob(uint64_t opcode, fv_rd rd, fv_rn rn, fv_rm rm, fv_imms imms, fv_immr immr, fv_immLarge immLarge)
    {
        cOperand_arm64* outOp = new cOperand_arm64();
        outOp->parsedOpcode.opcode = opcode;
        // FIXVAR_ADD(rd, outOp);
        outOp->fixvar_add(&outOp->parsedOpcode.rd, rd, e_rd);
        FIXVAR_ADD(rn, outOp);
        FIXVAR_ADD(rm, outOp);
        FIXVAR_ADD(imms, outOp);
        FIXVAR_ADD(immr, outOp);
        FIXVAR_ADD(immLarge, outOp);
        return outOp;
    }

    // example:
    // b 0x4000
    static uint64_t B_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP0_INST(lop, BR, B);
        return lop.opcode;
    }

    template <typename fv_immLarge>
    static cOperand* createB(fv_immLarge immLarge)
    {
        return insertToGlob<size_t, size_t, size_t, size_t, size_t, fv_immLarge>(
            B_inst(), 0, 0, 0, 0, 0, immLarge);
    }

    // example:
    // bl 0x4000
    static uint64_t BL_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP0_INST(lop, BR, BL);
        return lop.opcode;
    }

    template <typename fv_immLarge>
    static cOperand* createBL(fv_immLarge immLarge)
    {
        return insertToGlob<size_t, size_t, size_t, size_t, size_t, fv_immLarge>(
            BL_inst(), 0, 0, 0, 0, 0, immLarge);
    }

    // example: technically pc relative
    // ldr x5, #0x4000
    static uint64_t LDRL_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP2_INST(lop, LS, RL, NULL, IMM);
        return lop.opcode;
    }

    // create load register literal
    template <typename fv_rt, typename fv_imm19>
    static cOperand* createLDRL(fv_rt rt, fv_imm19 imm19)
    {
        return insertToGlob<fv_rt, size_t, size_t, size_t, size_t, fv_imm19>(
            LDRL_inst(), rt, 0, 0, 0, 0, imm19);
    }

    // example:
    // ldrb w14, [x10, #0x1]!
    static uint64_t LDRB_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP2_INST(lop, LS, RI, NULL, UIMM);
        return lop.opcode;
    }

    // example:
    // ldr x9, [x9, #0x1c0]
    static uint64_t LDRRRI_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP2_INST(lop, LS, RI64, NULL, IMM);
        return lop.opcode;
    }

    // create load/store register unsigned immediate
    template <typename fv_rt, typename fv_rn, typename fv_imm12>
    static cOperand* createLDRB(fv_rt rt, fv_rn rn, fv_imm12 imm12)
    {
        return insertToGlob<fv_rt, fv_rn, size_t, size_t, size_t, fv_imm12>(
            LDRB_inst(), rt, rn, 0, 0, 0, imm12);
    }

    // example: right now the add and subtract are interchangeable
    // add x8, x8, #0x1
    // sub x12, x12, #0x1
    static uint64_t ASI_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP0_INST(lop, DPIMM, ASI);
        return lop.opcode;
    }

    // create add subtract immediate
    template <typename fv_rd, typename fv_rn, typename fv_imm12>
    static cOperand* createASI(fv_rd rd, fv_rn rn, fv_imm12 imm12)
    {
        return insertToGlob<fv_rd, fv_rn, size_t, size_t, size_t, fv_imm12>(
            ASI_inst(), rd, rn, 0, 0, 0, imm12);
    }

    // example:
    // adrp x9, 0x40000
    static uint64_t ADRP_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP0_INST(lop, DPIMM, PC);
        return lop.opcode;
    }

    template <typename fv_rd, typename fv_immLarge>
    static cOperand* createADRP(fv_rd rd, fv_immLarge immLarge)
    {
        return insertToGlob<fv_rd, size_t, size_t, size_t, size_t, fv_immLarge>(
            ADRP_inst(), rd, 0, 0, 0, 0, immLarge);
    }

    // example: performs bitwise op on imm
    // and x8, x9, 0x04
    static uint64_t LI_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP0_INST(lop, DPIMM, LI);
        return lop.opcode;
    }

    // logical immediate, which in the end looks like a move
    template <typename fv_rd, typename fv_rn, typename fv_imms, typename fv_immr>
    static cOperand* createLI(fv_rd rd, fv_rn rn, fv_imms imms, fv_immr immr)
    {
        return insertToGlob<fv_rd, fv_rn, size_t, fv_imms, fv_immr, size_t>(
            LI_inst(), rd, rn, 0, imms, immr, 0);
    }

    // example: i BELIEVE the following:
    // movz w8, #0x115c
    static uint64_t MWI_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP0_INST(lop, DPIMM, MWI);
        return lop.opcode;
    }

    // mov with a wide immediate
    template <typename fv_rd, typename fv_imm16>
    static cOperand* createMWI(fv_rd rd, fv_imm16 imm16)
    {
        return insertToGlob<fv_rd, size_t, size_t, size_t, size_t, fv_imm16>(
            MWI_inst(), rd, 0, 0, 0, 0, imm16);
    }

    // example:
    // orr x8, x9, x8
    static uint64_t ORRR_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP2_INST(lop, DPREG, 2SRC, SER, LSR);
        return lop.opcode;
    }

    // orr reg, reg, reg
    template <typename fv_rd, typename fv_rn, typename fv_imm6, typename fv_rm>
    static cOperand* createORRR(fv_rd rd, fv_rn rn, fv_imm6 imm6, fv_rm rm)
    {
        return insertToGlob<fv_rd, fv_rn, fv_rm, size_t, size_t, fv_imm6>(
            ORRR_inst(), rd, rn, rm, 0, 0, imm6);
    }

    // alias for ORR register, where it is
    // mov reg, reg
    template <typename fv_rd, typename fv_rm>
    static cOperand* createMOV(fv_rd rd, fv_rm rm)
    {
        return createORRR<fv_rd, size_t, size_t, fv_rm>(rd, -1, 0, rm);
    }

    // example:
    // 
    static uint64_t ASSR_inst()
    {
        hdea64_opcode lop = {0};
        ENCODE_OP2_INST(lop, DPREG, 2SRC, SER, ASS);
        return lop.opcode;
    }

    // add reg, reg, reg
    template <typename fv_rd, typename fv_rn, typename fv_imm6, typename fv_rm>
    static cOperand* createASSR(fv_rd rd, fv_rn rn, fv_imm6 imm6, fv_rm rm)
    {
        return insertToGlob<fv_rd, fv_rn, fv_rm, size_t, size_t, fv_imm6>(
            ASSR_inst(), rd, rn, rm, 0, 0, imm6);
    }
};

// instSet<cOperand_arm64, val_set_A64_t>* createA64_instSet()
// {
//     instSet<cOperand_arm64, val_set_A64_t>* temp = new instSet<cOperand_arm64, val_set_A64_t>();
//     return temp;
// }

// #include "opcOp_arch.hpp"

#endif // OPCOP_AARCH64_H
