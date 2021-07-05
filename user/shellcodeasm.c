#include "kernel/types.h"
#include "user/user.h"

void
main()
{
__asm__(
    "j 0x2e            \n"
    "addi a0, ra, 8    \n"
    "sb a0, 11(ra)     \n"
    "addi a1, a0, 2    \n"
    "sb a1, 12(ra)     \n"
    "addi a1, a1, 1    \n"
    "li a7, 7          \n"
    "ecall             \n"
    "ret               \n"
    "li a0, 0          \n"
    "li a7, 2          \n"
    "ecall             \n"
    "ret               \n"
    "call 0xa          \n"
    ".string \"sh \"   \n"
);
}
