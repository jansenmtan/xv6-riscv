#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAX_ATTEMPTS 3
#define MAXLEN 30

int 
main(int argc, char *argv[]) {
  char username[MAXLEN];
  char password[PASSWORD_LEN];
  struct passwd *p;
  int attempt = 0;

  // Check if password file exists, if not, create root user
  fd = open("pwd", O_RDONLY);
  if (fd < 0) {
    // Password file does not exist, create root user
    printf("Creating root user...\n");

    // Prompt for root username
    printf("Enter root username: ");
    gets(username);

    // Prompt for root password
    printf("Enter root password: ");
    gets(password);

    struct passwd root_user;
    root_user.pw_name = username;
    root_user.pw_passwd = password; // Store the password directly
    root_user.pw_uid = 0;
    root_user.pw_gid = 0;
    root_user.pw_dir = "/root"; // Default home directory for root
    root_user.pw_shell = "/bin/sh"; // Default shell for root

    // Write the root user to the password file
    fd = open("pwd", O_WRONLY | O_CREAT);
    if (fd < 0) {
      printf("Failed to create password file.\n");
      exit(1);
    }
    putpwent(&root_user, fd);
    close(fd);
  } else {
    close(fd);
  }

  while (attempt < MAX_ATTEMPTS) {
    // Prompt for username
    printf("Username: ");
    gets(username);

    // Prompt for password
    printf("Password: ");
    gets(password);

    // Authenticate user given input
    p = getpwnam(username);
    if (p != NULL && strcmp(p->pw_passwd, password) == 0) {
      // Upon successful authentication: set process UID, GID to identity
      setuid(p->pw_uid);
      setgid(p->pw_gid);

      // Upon successful authentication: execute shell
      char *args[] = {p->pw_shell, NULL};
      exec(p->pw_shell, args);
    } else {
      printf("Login failed.\n");
      attempt++;
    }
  }

  printf("Too many failed login attempts.\n");
  exit(1);
}


