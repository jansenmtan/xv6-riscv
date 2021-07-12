#include "kernel/types.h"
#include "user/user.h"

void
main()
{
__asm__(
    "jal t0, 0x2a       \n"
    "addi a0, ra, 100   \n"
    "addi a0, a0, -100  \n"
    "sd a0, 8(a0)       \n"
    "addi a1, a0, -2    \n"
    "addi a1, a1, 10    \n"
    "xor a6, a6, a6     \n"
    "sd a6, 16(a0)      \n"
    "jalr a6, a6, 0x2e0 \n"
    "xor a6, a6, a6     \n"
    "jal ra, 0xa        \n"
    ".string \"sh \"    \n"
);
}
