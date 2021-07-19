#include "kernel/types.h"
#include "user/user.h"

void
vulnerable_function()
{
  char buf[48];
  int fd, n;

  printf("buf: %p\n", buf);

  if((fd = open("README", 0)) < 0){
    printf("failed to open README");
    exit(1);
  }

  while((n = read(fd, buf, (80)*sizeof(char))) > 0);
  if(n < 0){
    printf("failed to read README");
    exit(1);
  }

  printf("%s\n", buf);
}

void
main(int argc, char *argv[])
{
  vulnerable_function();
}
