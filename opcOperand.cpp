#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <localUtil.h>

#ifdef LIVE_KERNEL
#include <hdeA64.h>
#else
#include <capstone/capstone.h>
#endif

#include "opcOperand.h"

void cOperand::clearVars()
{
    fixvar_set = fixvar_unmod;
}
