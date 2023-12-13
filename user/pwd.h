#define PASSWD_PATH "/password.txt"

struct passwd {
  char *pw_name;   // Username
  char *pw_passwd; // User password
  uid_t pw_uid;    // User ID
  gid_t pw_gid;    // Group ID
  char *pw_gecos;  // User information
  char *pw_dir;    // Home directory
  char *pw_shell;  // Shell program
};


// taken from man://pwd.h (POSIX)
struct passwd *getpwent(void);
void setpwent(void);
void endpwent(void);
struct passwd *getpwnam(const char *);
// int getpwnam_r(const char *, struct passwd *, char *, size_t, struct passwd **);
struct passwd *getpwuid(uint);
// int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **);
int putpwent(const struct passwd *);

