#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include <array>

#include <hdeA64.h>
#include <hde.h>
#include "arm64/opcOp_arch.h"
#include "amd64/opcOp_arch.h"
#include "ibeSet.h"

typedef instSet<cOperand_arm64, val_set_A64_t> instSetA;
typedef cOperand_arm64::saveVar_t saveVar_t;

int processreg(std::string inreg)
{
#define PR_ELIF(N) \
    else if (inreg == "x" # N) \
    { \
        procreg = X ## N; \
    }

    int procreg = -1;

    if (inreg == "x0")
    {
        procreg = X0;
    }
    PR_ELIF(1)
    PR_ELIF(2)
    PR_ELIF(3)
    PR_ELIF(4)
    PR_ELIF(5)
    PR_ELIF(6)
    PR_ELIF(7)
    PR_ELIF(8)
    PR_ELIF(9)
    PR_ELIF(10)
    PR_ELIF(11)
    PR_ELIF(12)
    PR_ELIF(13)
    PR_ELIF(14)
    PR_ELIF(15)
    PR_ELIF(16)
    PR_ELIF(17)
    PR_ELIF(18)
    PR_ELIF(19)
    PR_ELIF(20)
    PR_ELIF(21)
    PR_ELIF(22)
    PR_ELIF(23)
    PR_ELIF(24)
    PR_ELIF(25)
    PR_ELIF(26)
    PR_ELIF(27)
    PR_ELIF(28)
    PR_ELIF(29)
    PR_ELIF(30)

    return procreg;
}

void processrega64(std::string inreg)
{
#define PR_ELIFX64(N) \
    else if (inreg == "R" # N) \
    { \
        procreg = R ## N; \
    }
    int procreg = -1;

    if (inreg == "RAX")
    {
        procreg = RAX;
    }
    PR_ELIFX64(CX)
    PR_ELIFX64(DX)
    PR_ELIFX64(BX)
    PR_ELIFX64(SP)
    PR_ELIFX64(BP)
    PR_ELIFX64(SI)
    PR_ELIFX64(DI)
    PR_ELIFX64(8)
    PR_ELIFX64(9)
    PR_ELIFX64(10)
    PR_ELIFX64(11)
    PR_ELIFX64(12)
    PR_ELIFX64(13)
    PR_ELIFX64(14)
    PR_ELIFX64(15)
    else
    {
        if (inreg.substr(0, 4) == "RVAR")
        {
            
        }
    }
}

enum ARGTYPE { IMM, VAR, REG };

class argdef
{
public:
    ARGTYPE argtype;
};

template <typename T>
class argtyped : public argdef
{
public:
    T storage;
    argtyped(T tin, ARGTYPE argtin) : storage(tin) { argtype = argtin; };
};

int processarg(std::string arg, argdef** argdef_out)
{
    int result = -1;

    SAFE_BAIL(argdef_out == 0);

    if (arg[0] == '#')
    {
        argtyped<size_t>* argdef_tmp = new argtyped<size_t>(strtoull(&arg.data()[1], NULL, 0x10), IMM);
        *argdef_out = argdef_tmp;
        result = 0;
    }
    else if (arg[0] == '$')
    {
        argtyped<std::string>* argdef_tmp = new argtyped<std::string>(arg, VAR);
        *argdef_out = argdef_tmp;
        result = 0;
    }
    else
    {
        argtyped<size_t>* argdef_tmp = new argtyped<size_t>(processreg(arg), REG);
        *argdef_out = argdef_tmp;
        result = 0;
    }
fail:
    return result;
}

cOperand_arm64* createinst(uint64_t opcode, std::vector<std::string>* variable_keys, std::vector<operand_val<val_set_A64_t>*>* oparr)
{
    cOperand_arm64* outOp = new cOperand_arm64();
    operand_val<val_set_A64_t>* temp_op = 0;
    
    for (int i = 0; i < oparr->size(); i++)
    {
        temp_op = (*oparr)[i];
        outOp->parsedOpcode.opcode = opcode;

        if (temp_op->vari == true)
        {
            if (temp_op->val_set == e_rd)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.rd, ((operand_val_typed<saveVar_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_rn)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.rn, ((operand_val_typed<saveVar_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_rm)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.rm, ((operand_val_typed<saveVar_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_imms)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.imms, ((operand_val_typed<saveVar_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_immr)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.immr, ((operand_val_typed<saveVar_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_immLarge)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.immLarge, ((operand_val_typed<saveVar_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
        }
        else
        {
            if (temp_op->val_set == e_rd)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.rd, ((operand_val_typed<size_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_rn)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.rn, ((operand_val_typed<size_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_rm)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.rm, ((operand_val_typed<size_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_imms)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.imms, ((operand_val_typed<size_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_immr)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.immr, ((operand_val_typed<size_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
            else if (temp_op->val_set == e_immLarge)
            {
                outOp->fixvar_add(&outOp->parsedOpcode.immLarge, ((operand_val_typed<size_t, val_set_A64_t>*)temp_op)->storage, temp_op->val_set);
            }
        }
    }
    return outOp;
}

int assign_args(instSetA* getB_in, std::vector<std::string>* variable_keys, std::vector<argdef*>* argsarr, std::vector<operand_val<val_set_A64_t>*>* opvalarr, std::vector<val_set_A64_t>* val_types)
{
    argdef* curargdef;
    operand_val<val_set_A64_t>* curopval;
    operand_val<val_set_A64_t>** curopval_out;
    val_set_A64_t paramtype;

    for (int argar_index = 0; argar_index < argsarr->size(); argar_index++)
    {
        curargdef = (*argsarr)[argar_index];
        curopval_out = &(*opvalarr)[argar_index];
        paramtype = (*val_types)[argar_index];

        if (curargdef->argtype == IMM)
        {
            curopval = new operand_val_typed<size_t, val_set_A64_t>(((argtyped<size_t>*)curargdef)->storage, false, paramtype);
        }
        if (curargdef->argtype == VAR)
        {
            auto varkeyit = std::find(variable_keys->begin(), variable_keys->end(), ((argtyped<std::string>*)curargdef)->storage);
            if (varkeyit == variable_keys->end())
            {
                variable_keys->push_back(((argtyped<std::string>*)curargdef)->storage);
                curopval = new operand_val_typed<saveVar_t, val_set_A64_t>(getB_in->checkOperand(variable_keys->size() - 1), true, paramtype);
            }
            else
            {
                int indexcount = 0;
                for (auto i = variable_keys->begin(); i != variable_keys->end(); i++)
                {
                    if (i == varkeyit)
                    {
                        break;
                    }
                    indexcount++;
                }
                curopval = new operand_val_typed<saveVar_t, val_set_A64_t>(getB_in->checkOperand(indexcount), true, paramtype);
            }
        }
        if (curargdef->argtype == REG)
        {
            curopval = new operand_val_typed<size_t, val_set_A64_t>(((argtyped<size_t>*)curargdef)->storage, false, paramtype);
        }
        *curopval_out = curopval;
    }

    return 0;
}

void arm64_parseNargs(instSetA* getB_in, uint64_t opcode, std::vector<std::string>* operandlist, std::vector<std::string>* variable_keys, int argcount, ...)
{
    std::vector<argdef*> argdefarr(argcount);
    std::vector<operand_val<val_set_A64_t>*> oparr(argcount);
    std::vector<val_set_A64_t> val_types;
    va_list arguments;
    val_set_A64_t paramtype;

    va_start(arguments, argcount);
    for (int i = 0; i < argcount; i++)
    {
        val_types.push_back(va_arg(arguments, val_set_A64_t));
    }
    va_end(arguments);

    for (int i = 0; i < argcount; i++)
    {
        processarg((*operandlist)[i], &argdefarr[i]);

    }
    assign_args(getB_in, variable_keys, &argdefarr, &oparr, &val_types);
    getB_in->addNewInst(createinst(opcode, variable_keys, &oparr));
}

void arm64_parseinst(instSetA* getB_in, std::string opcode, std::vector<std::string>* operandlist, std::vector<std::string>* variable_keys)
{
    if (opcode == "b")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::B_inst(), operandlist, variable_keys, 1, e_immLarge);
    }
    else if (opcode == "bl")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::BL_inst(), operandlist, variable_keys, 1, e_immLarge);
    }
    else if (opcode == "ldrl")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::LDRL_inst(), operandlist, variable_keys, 2, e_rd, e_imm);
    }
    else if (opcode == "ldrb")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::LDRB_inst(), operandlist, variable_keys, 3, e_rd, e_rn, e_immLarge);
    }
    else if (opcode == "ldrrri")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::LDRRRI_inst(), operandlist, variable_keys, 3, e_rd, e_rn, e_immLarge);
    }
    else if (opcode == "asi")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::ASI_inst(), operandlist, variable_keys, 3, e_rd, e_rn, e_immLarge);
    }
    else if (opcode == "adrp")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::ADRP_inst(), operandlist, variable_keys, 2, e_rd, e_immLarge);
    }
    else if (opcode == "li")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::LI_inst(), operandlist, variable_keys, 4, e_rd, e_rn, e_imms, e_immr);
    }
    else if (opcode == "mwi")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::MWI_inst(), operandlist, variable_keys, 2, e_rd, e_immr);
    }
    else if (opcode == "orrr")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::ORRR_inst(), operandlist, variable_keys, 4, e_rd, e_rn, e_rm, e_immLarge);
    }
    else if (opcode == "mov")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::ORRR_inst(), operandlist, variable_keys, 4, e_rd, e_rn, e_rm, e_immLarge);
    }
    else if (opcode == "assr")
    {
        arm64_parseNargs(getB_in, cOperand_arm64::ASSR_inst(), operandlist, variable_keys, 4, e_rd, e_rn, e_rm, e_immLarge);
    }
}

std::string replacetar(std::string target, std::string torem, std::string replacement)
{
    int foundindex = 0;

    foundindex = target.find(torem);
    if (foundindex != -1)
    {
        target.replace(foundindex, torem.size(), replacement);
    }
    return target;
}

void processin(const char* regexstream, instSetA** getb_out)
{
    instSetA* getB = new instSetA(AARCH64_IBE);

    std::string regexstream_str = std::string(regexstream);
    std::istringstream ss(regexstream_str);
    std::string line;
    std::string opcode;
    std::vector<std::string> variable_keys;

    while (std::getline(ss, line, ';'))
    {
        std::stringstream linestream(line);
        std::string tmpoperand;
        std::vector<std::string> operandvec;
        linestream >> opcode;
        opcode = replacetar(opcode, ";", "");

        while (linestream >> tmpoperand)
        {
            tmpoperand = replacetar(tmpoperand, ",", "");
            tmpoperand = replacetar(tmpoperand, ";", "");
            
            operandvec.push_back(tmpoperand);
        }
        arm64_parseinst(getB, opcode, &operandvec, &variable_keys);
    }
    *getb_out = getB;
}
