#include <sys/syscall.h>
#include <stdio.h>

void main () {
    FILE *fptr;
    fptr = fopen('zoinks.txt', 'r');
    fclose(fptr);
}