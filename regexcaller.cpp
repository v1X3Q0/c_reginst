#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <localUtil.h>

#include "ibeSet.h"
#include "arm64/opcOp_arch.h"
#include "regexparse.h"

void usage(const char* arg)
{
    printf("%s: -i inputfile -e expr_file -r raw_expr -s skip -n numbytes -a all\n", arg);
    exit(1);
}

int main(int argc, char** argv)
{
    int opt = 0;
    const char* infile = 0;
    const char* expr_file = 0;
    void* searchbase = 0;
    void* searchcur = 0;
    size_t searchsz = 0;
    size_t searchszin = 0;
    instSet<cOperand_arm64, val_set_A64_t>* getB = 0;
    void* resaddr = 0;
    const char* expr_raw = 0;
    int offset = 0;
    int findall = 0;
    
    while ((opt = getopt(argc, argv, "i:e:r:s:n:a")) != -1)
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
        case 's':
            offset = strtoull(optarg, NULL, 0x10);
            break;
        case 'n':
            searchszin = strtoull(optarg, NULL, 0x10);
            break;
        case 'a':
            findall = 1;
            break;
        default: /* '?' */
            usage(argv[0]);
        }
    }

    if (optind < argc)
    {
        usage(argv[0]);
    }

    // some sample infile's and args
    // infile = "/Users/mariomain/projects/huawei/mate20/lya-l29_9.1.0.300/kernel_extracted.bin";
    // infile="/Users/mariomain/projects/huawei/p50/_BOOT.img.extracted/1000";
    // expr_raw = "adrp $reg2, $const2; ldrrri $reg2, $reg2, $const1; adrp $reg3, $const3; ldrrri $reg3, $reg3, $const4;";
    SAFE_BAIL(block_grab(infile, &searchbase, &searchsz) == -1);
    if (searchszin != 0)
    {
        searchsz = searchszin;
    }
    else
    {
        searchsz = searchsz - offset;
    }
    if (expr_file != 0)
    {
        SAFE_BAIL(block_grab(expr_file, (void**)&expr_raw, NULL) == -1);
    }

    processin(expr_raw, &getB);
    searchcur = (void*)((uint8_t*)searchbase + offset);
    do
    {
        SAFE_BAIL(getB->findPattern((uint8_t*)searchcur, searchsz, &resaddr) == -1);
        printf("found at offset 0x%08lx\n", ((size_t)resaddr - (size_t)searchbase));
        // searchcur = resaddr + cOperand_arm64::getinstsz();
        searchsz = searchsz - ((size_t)resaddr - (size_t)searchcur);
        searchcur = (void*)((size_t)resaddr + 4);
    } while (findall == 1);
    

fail:
    SAFE_FREE(searchbase);
    if (expr_file != 0)
    {
        SAFE_FREE(expr_raw);
    }
    SAFE_DEL(getB);
    return 0;
}
