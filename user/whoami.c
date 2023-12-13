#include "kernel/types.h"
#include "user/user.h"
#include "user/pwd.h"

int
main(int argc, char *argv[])
{
  // TODO: implement `whoami`
  // Retrieve the current user's UID and get the corresponding name
  p = getpwuid(getuid());
  if (p != NULL) {
    printf("%s\n", p->pw_name);
  } else {
    printf("Unknown user.\n");
  }

  exit(0);

}
