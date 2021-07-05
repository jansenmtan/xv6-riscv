#include "kernel/types.h"
#include "user/user.h"

void
main()
{
    char *argv[2];

    argv[0] = "sh";
    argv[1] = 0;
    exec(argv[0], argv);
}
