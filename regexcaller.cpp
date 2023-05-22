#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <localUtil.h>

#include "ibeSet.h"
#include "arm64/opcOp_arch.h"
#include "regexparse.h"

void usage(const char* arg)
{
    printf("%s: -i inputfile -e expr_file", arg);
    exit(1);
}

int main(int argc, char** argv)
{
    int opt = 0;
    const char* infile = 0;
    const char* expr_file = 0;
    void* searchbase = 0;
    size_t searchsz = 0;
    instSet<cOperand_arm64, val_set_A64_t>* getB = 0;
    void* resaddr = 0;
    const char* expr_raw = 0;
    
    while ((opt = getopt(argc, argv, "i:e:r:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            infile = optarg;
            break;
        case 'e':
            expr_file = optarg;
            break;
        case 'r':
            expr_raw = optarg;
            break;
        default: /* '?' */
            usage(argv[0]);
        }
    }

    if (optind < argc)
    {
        usage(argv[0]);
    }

    SAFE_BAIL(block_grab(infile, &searchbase, &searchsz) == -1);
    if (expr_file != 0)
    {
        SAFE_BAIL(block_grab(expr_file, (void**)&expr_raw, NULL) == -1);
    }

    processin(expr_raw, &getB);
    getB->findPattern((uint8_t*)searchbase, searchsz, &resaddr);

fail:
    return 0;
}
