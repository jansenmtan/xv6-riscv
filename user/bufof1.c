#include "kernel/types.h"
#include "user/user.h"

void
give_shell()
{
  printf("executing shell\n");

  char *argv[2];

  argv[0] = "sh";
  argv[1] = 0;
  exec(argv[0], argv);
}

int
main(int argc, char *argv[])
{
  char buf[8];

  strcpy(buf, argv[1]);
  printf(buf);
  
  return 0;
}
