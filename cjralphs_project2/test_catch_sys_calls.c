#include <linux/syscalls.h>
#include <sys/stdio.h>

void main () {
    FILE *fptr;
    fptr = fopen('zoinks.txt', 'r');
    fclose(fptr);
}