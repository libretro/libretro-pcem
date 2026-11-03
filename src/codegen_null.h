/* The recompiler's block layout, for a host without a recompiler: the code
 * that tracks pages of code (mem.c, codegen.h) still uses these, though no
 * block is ever compiled. Same values as codegen_x86-64.h. */
#define BLOCK_SIZE 0x4000
#define BLOCK_MASK 0x3fff
#define BLOCK_START 0

#define HASH_SIZE 0x20000
#define HASH_MASK 0x1ffff

#define HASH(l) ((l) & 0x1ffff)

#define BLOCK_EXIT_OFFSET 0x7f0
#define BLOCK_GPF_OFFSET (BLOCK_EXIT_OFFSET - 20)
