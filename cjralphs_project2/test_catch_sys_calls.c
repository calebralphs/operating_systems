#include <sys/syscall.h>
#include <stdio.h>

void main () {
    FILE *fp;
    fp = fopen("zoinks.txt", "r");
    fclose(fp);
}
