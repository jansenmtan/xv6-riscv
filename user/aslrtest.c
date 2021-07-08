#include "kernel/types.h"
#include "user/user.h"

int y;

void
f2()
{
  int x;
  x = 3;
  printf("f2: stack: &x: %p\n", &x);
}

void
f1()
{
  int x;
  x = 2;
  printf("f1: stack: &x: %p\n", &x);
  f2();
}

int main(int argc, char *argv[])
{
  int x ;
  int *ptr;
  x = 10;
  y = 20;
  ptr = (int *)malloc(4);
  printf("main:      %p \n", main);
  printf("stack: &x: %p \n", &x);
  printf("stack: &y: %p \n", &y);
  printf("heap: ptr: %p \n", ptr);
  f1();
  exit(0);
}
