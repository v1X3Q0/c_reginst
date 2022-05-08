#include <vector>
#include <list>

#include <localUtil.h>
#include "ibeSet.h"
#include "opcOperand.h"
#include "arm64/opcOp_arch.h"

void instSet::addNewInst(cOperand* newInstruction)
{
    instPatternList.push_back(newInstruction);   
}

void instSet::clearInternals()
{
    auto it = varTable.begin();

    while (instPatternList.size() > 0)
    {
        SAFE_DEL(instPatternList.front());
        instPatternList.pop_front();
    }

    while (varTable.size() > 0)
    {
        it = varTable.begin();
        SAFE_FREE(it->second);
        varTable.erase(it->first);
    }
}

saveVar_t instSet::checkOperand(uint32_t newOperand)
{
    saveVar_t result = 0;
    auto it = varTable.begin();
    it = varTable.find(newOperand);
    if (it == varTable.end())
    {
        result = (saveVar_t)calloc(1, sizeof(saveVar));
        varTable[newOperand] = result;
    }
    result = varTable[newOperand];
    return result;
}

saveVar_t instSet::addOperand(uint32_t newOperand, val_set_t val_set, cOperand* regRand)
{
    saveVar_t result = 0;
    auto it = varTable.begin();
    it = varTable.find(newOperand);
    if (it == varTable.end())
    {
        result = (saveVar_t)calloc(1, sizeof(saveVar));
        result->val_set = val_set;
        result->regRand = regRand;
        varTable[newOperand] = result;
    }
    result = varTable[newOperand];
    return result;
}

int instSet::getVar(uint32_t key, size_t* value)
{
    int result = -1;
    auto i = varTable.begin();

    SAFE_BAIL(value == 0);
    i = varTable.find(key);
    SAFE_BAIL(i == varTable.end());

    SAFE_BAIL(i->second->regRand->getOpComp(i->second->val_set, value) == -1);

    result = 0;
fail:
    return result;
}

bool equalInstSet(std::list<cOperand*>* list1, std::list<cOperand*>* list2)
{
    auto it1 = list1->begin();
    auto it2 = list2->begin();

    for (; it1 != list1->end(); it1++, it2++)
    {
        if ((*it1)->checkHelper((*it2)) == false)
        {
            return false;
        }
    }
    return true;
}

void instSet::clearVars()
{
    for (auto i = varTable.begin(); i != varTable.end(); i++)
    {
        i->second->regRand->clearVars();
    }
}

int instSet::incrementInst(uint8_t* curAddr, cOperand* curInst, uint8_t** resultAddr)
{
    int result = -1;
    // variable length inst sets can't really do much for step, could work
    // if going forward, but will have to do something funny for backwards.
    if ((ibe_arch == I386_IBE) || (ibe_arch == X86_64_IBE))
    {
        *resultAddr = curAddr + curInst->getinstsz();
    }
    else
    {
        *resultAddr = curAddr + curInst->getinstsz() * step;
    }

    result = 0;

    return result;
}

int instSet::findPattern_fixed(uint8_t* startAddress_a, size_t sizeSearch, void** resultAddr_a)
{
    int result = -1;
    uint8_t* startAddress = startAddress_a;
    uint8_t** resultAddr = (uint8_t**)resultAddr_a;
    uint8_t* endAddress = startAddress + sizeSearch;
    uint8_t* curAddr = startAddress;
    uint8_t* slideEnd = 0;
    std::list<cOperand*> instSlide;
    cOperand* tmpInst = 0;
    int dbgCounter = 0;
    
    for (int i = 0; i < instPatternList.size(); i++)
    {
        if (ibe_arch == AARCH64_IBE)
        {
            tmpInst = cOperand::createOp<cOperand_arm64>(curAddr);
        }
        if (step > 0)
        {
            instSlide.push_back(tmpInst);
        }
        else
        {
            instSlide.push_front(tmpInst);
        }
        
        incrementInst(curAddr, tmpInst, &curAddr);
    }

    slideEnd = curAddr;

    for (curAddr = startAddress; evaluateFinish(slideEnd, endAddress) == false; dbgCounter++)
    {
        FINISH_IF(equalInstSet(&instPatternList, &instSlide) == true);
        clearVars();

        if (step > 0)
        {
            tmpInst = instSlide.front();
            instSlide.pop_front();
            instSlide.push_back(tmpInst);
        }
        else
        {
            tmpInst = instSlide.back();
            instSlide.pop_back();
            instSlide.push_front(tmpInst);
        }

        // increments
        incrementInst(curAddr, instSlide.front(), &curAddr);
        tmpInst->initme(slideEnd);
        incrementInst(slideEnd, tmpInst, &slideEnd);
    }
    goto fail;
    
finish:
    result = 0;
    *resultAddr = curAddr;

fail:
    while (instSlide.size() != 0)
    {
        SAFE_DEL(instSlide.front());
        instSlide.pop_front();
    }

    return result;
}
