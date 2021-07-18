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

void
main(int argc, char *argv[])
{
  char buf[64];
  int fd;

  printf("buf: %p\n", buf);

  if((fd = open("README", 0)) < 0)
    exit(1);
  while((read(fd, buf, sizeof(char)) == sizeof(char)));
  printf("%s\n", buf);

  exit(0);
}
