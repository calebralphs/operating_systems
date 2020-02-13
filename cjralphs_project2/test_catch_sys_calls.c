#include <sys/syscall.h>
#include <stdio.h>

int main () {
    FILE *fptr;
    fptr = fopen('zoinks.txt', 'r');
    fclose(fptr);
}