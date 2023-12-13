#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAXLEN 128 
#define PASSWORD_LEN 8

uint
create_uid(void)
{
  struct passwd *p;
  int uid = 0; // first user created will be root (uid 0)
  while((p = getpwuid(uid))){
    free(p);
    uid++;
  }
  return uid;
}

int 
main(int argc, char *argv[]) {
  char username[MAXLEN];
  char password[PASSWORD_LEN];
  int fd;

  // Prompt for username
  printf("Enter username: ");
  gets(username);

  // Check if user already exists
  if (getpwnam(username) != NULL) {
    printf("User already exists.\n");
    exit(1);
  }

  // Prompt for password
  printf("Enter password: ");
  gets(password);

  // Hash and salt password
  // TODO: Implement a hashing function

  // Create user account entry
  struct passwd new_user;
  new_user.pw_name = username;
  new_user.pw_passwd = password; // Store the hashed password
  new_user.pw_uid = create_uid();
  new_user.pw_gid = new_user.pw_uid; // For simplicity, UID and GID are the same
  new_user.pw_dir = "/home"; // Default home directory
  new_user.pw_shell = "/bin/sh"; // Default shell

  // Write the new user to the password file
  fd = open(PASSWD_PATH, O_WRONLY | O_APPEND);
  if (fd < 0) {
    printf("Failed to open password file.\n");
    exit(1);
  }
  putpwent(&new_user, fd);
  close(fd);

  printf("User added successfully.\n");
  exit(0);
}
