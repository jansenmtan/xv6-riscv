#include "kernel/types.h"
#include "user/user.h"

void
main()
{
__asm__(
    "j 0x2c            \n"
    "addi a0, ra, 0    \n"
    "sd a0, 8(ra)      \n"
    "sd zero, 16(ra)   \n"
    "addi a1, ra, 8    \n"
    "li a7, 7          \n"
    "ecall             \n"
    "ret               \n"
//     "li a0, 0          \n"
//     "li a7, 2          \n"
//     "ecall             \n"
//     "ret               \n"
    "call 0xa          \n"
    ".string \"sh \"   \n"
);
}
