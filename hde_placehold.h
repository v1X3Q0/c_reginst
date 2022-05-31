#ifndef HDE_PLACEHOLD_H
#define HDE_PLACEHOLD_H

typedef enum
{
    e_rd=1,
    e_rn=e_rd << 1,
    e_rm=e_rn << 1,
    e_imms=e_rm << 1,
    e_immr=e_imms << 1,
    e_immLarge=e_immr << 1
} val_set_t;

#define VAL_SET_REGMASK (e_rd | e_rn | e_rm)

typedef struct
{
    
} hde_t;

#endif