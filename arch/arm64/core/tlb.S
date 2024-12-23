/*
 * Copyright 2024 HNU-ESNL
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <toolchain.h>
#include <linker/sections.h>
#include <virtualization/arm/asm.h>

#include "../include/zvm_offsets_short_arch.h"
#include "macro_priv.inc"

 _ASM_FILE_PROLOGUE

 /**
 * @brief invalidate all tlb entries.
 */
GTEXT(__asm_invalidate_tlb_all)
SECTION_SUBSEC_FUNC(TEXT, tlb_section_0,__asm_invalidate_tlb_all)
    cmp     x0, #1
    b.eq    1f
    cmp     x0, #2
    b.eq    2f
    b       0f

2:  tlbi    alle2
    dsb     sy
    isb
    b       0f

1:  tlbi    alle1
    dsb     sy
    isb

0:  ret
