#include <string>
#include <hdeA64.h>
#include <hde.h>
#include "arm64/opcOp_arch.h"
#include "amd64/opcOp_arch.h"
#include "ibeSet.h"

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

void processfunc(std::string infunc)
{
    instSet<cOperand_arm64, val_set_A64_t>* getB = new instSet<cOperand_arm64, val_set_A64_t>();

    if (infunc == "b")
    {
        processreg(infunc);
    }
    else if (infunc == "bl")
    {
        getB->addNewInst(cOperand_arm64::createBL<saveVar_t>(getB.checkOperand(3)));

        getB->addNewInst(cOperand_arm64::createBL(getB. strtoull(infunc.c_str(), NULL, 0x10)));
    }
    else if (infunc == "ldrl")
    {
        cOperand_arm64::createLDRL(processreg(infunc), strtoull(infunc.c_str(), NULL, 0x10));
    }
}