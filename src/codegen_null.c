/* The recompiler's back end on a host it was not written for (ARM64 and
 * others): nothing is ever compiled - the core keeps cpu_use_dynarec at 0,
 * so the interpreter (386.c) runs every CPU - but 386_dynarec.c, which also
 * holds state the interpreter shares, links against these. */
#include "ibm.h"
#include "cpu.h"
#include "x86.h"
#include "x86_ops.h"
#include "mem.h"
#include "codegen.h"

int codegen_flags_changed;
int codegen_fpu_entered;
int codegen_fpu_loaded_iq[8];
int codegen_reg_loaded[8];
int codegen_mmx_entered;
int op_ssegs;
uint32_t op_old_pc;
uint32_t recomp_page = -1;
codeblock_t *codeblock;
codeblock_t **codeblock_hash;
int block_current;
int block_pos;
int cpu_recomp_flushes, cpu_recomp_flushes_latched;
int cpu_recomp_evicted, cpu_recomp_evicted_latched;
int cpu_recomp_reuse, cpu_recomp_reuse_latched;
int cpu_recomp_removed, cpu_recomp_removed_latched;
int codegen_block_cycles;

void codegen_init()
{
}

void codegen_reset()
{
}

void codegen_block_init(uint32_t phys_addr)
{
        (void)phys_addr;
}

void codegen_block_remove()
{
}

void codegen_block_end()
{
}

void codegen_generate_call(uint8_t opcode, OpFn op, uint32_t fetchdat, uint32_t new_pc, uint32_t old_pc)
{
        (void)opcode; (void)op; (void)fetchdat; (void)new_pc; (void)old_pc;
}

void codegen_generate_seg_restore()
{
}

void codegen_check_abrt()
{
}

void codegen_set_op32()
{
}

void codegen_flush()
{
}

void codegen_check_flush(struct page_t *page, uint64_t mask, uint32_t phys_addr)
{
        (void)page; (void)mask; (void)phys_addr;
}
