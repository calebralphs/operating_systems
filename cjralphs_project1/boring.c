// Caleb Ralphs
#include "boring.h"

void main(void) {
    char* cmd1 = "whoami";
    char* cmd2 = "last";
    char* cmd3 = "ls -al /home";
    execCommand(cmd1);
    execCommand(cmd2);
    execCommand(cmd3);
}