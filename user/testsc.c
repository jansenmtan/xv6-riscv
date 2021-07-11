#include "kernel/types.h"
#include "user/user.h"

void
main()
{
char shellcode[] =
//     "\x6f\x00\x80\x02"
//     "\x13\x85\x80\x00"
//     "\xa3\x85\xa0\x00"
//     "\x93\x05\x25\x00"
//     "\x23\x86\xb0\x00"
//     "\x85\x05"
//     "\x9d\x48"
//     "\x73\x00\x00\x00"
//     "\x82\x80"
    "\x01\x45"
    "\x89\x48"
    "\x73\x00\x00\x00"
//     "\x82\x80"
//     "\x97\x00\x00\x00"
//     "\xe7\x80\xc0\xfd"
//     "\x73\x68\x00\x00"
    ;

    int *ret;

    ret = (int *)&ret + 2;
    (*ret) = (int)shellcode;
}
