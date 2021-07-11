#include "kernel/types.h"
#include "user/user.h"

void
main()
{
    char *argv[2];

    argv[0] = "sh";
    argv[1] = 0;
//     unsigned long argv[] =
//         {0x00006873, 0x00000000};

    exec(argv[0], argv);
}
