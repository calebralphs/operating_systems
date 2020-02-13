#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include "part2.h"

#define __NR_cs3013_syscall2 378

void main(int argc, char* argv[]) {
    if (argc != 2) {
        // grrrrr
        exit(-1);
    }
    int pid = atoi(argv[1]);
    // test sys_cs3013_syscall1
    syscall(__NR_cs3013_syscall2, pid);
}