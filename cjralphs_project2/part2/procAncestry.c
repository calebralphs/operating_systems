#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "part2.h"

#define __NR_cs3013_syscall2 378

void main(int argc, char* argv[]) {
    if (argc != 2) {
        // grrrrr
        exit(-1);
    }
    int i;
    unsigned short pid = (unsigned short)atoi(argv[1]);
    struct ancestry response;
    // test sys_cs3013_syscall1
    syscall(__NR_cs3013_syscall2, &pid, &response);
    for (i = 0; i < 10; i++) {
        printf("(User Space) Ancestor %d: %d\n", i, response.ancestors[i]);
    }
    for (i = 0; i < 100; i++) {
        printf("(User Space) Child %d: %d\n", i, response.children[i]);
    }
    for (i = 0; i < 100; i++) {
        printf("(User Space) Sibling %d: %d\n", i, response.siblings[i]);
    }
}
