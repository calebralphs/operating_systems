#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define __NR_cs3013_syscall1 377

void main () {
    // test  sys_open, sys_close
    FILE *fp;
    fp = fopen("zoinks.txt", "r");
    fclose(fp);

    // test sys_cs3013_syscall1
    syscall(__NR_cs3013_syscall1);
}
