#ifndef REGEXPARSE_H
#define REGEXPARSE_H
#include "ibeSet.h"
#include "arm64/opcOp_arch.h"

void processin(const char* regexstream, instSet<cOperand_arm64, val_set_A64_t>** getb_out);

#endif // REGEXPARSE_H