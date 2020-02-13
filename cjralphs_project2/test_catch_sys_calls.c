#include <linux/syscalls.h>
#include <linux/stdio.h>

void main () {
    FILE *fptr;
    fptr = fopen('zoinks.txt', 'r');
    fclose(fptr);
}