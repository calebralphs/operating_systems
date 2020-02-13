#include "multi.h"


void handleBackground(char* filename, int argc, char* argv[]) {
    int i = 0;
    int* lineNums = malloc((argc-1) * sizeof(int));
    for (i; i < argc - 1; i++){
        lineNums[i] = atoi(argv[i+1]);
    }

    struct timeval clock_init;
    struct timeval clock_end;
    struct rusage rusage_info;
    long pageFault_init, pageFault_end, pageFaultReclaimed_init, pageFaultReclaimed_end;

    FILE* file = fopen(filename, "r");
    char line[MAXCHAR];

    int* background_pids = malloc((argc-2) * sizeof(int));
    int lineNums_idx = 0;
    int currLine = 0;
    int doneLineNums = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strlen(line)-2] = 0;
        printf("Running command: %s\n", line);

        char* argv[MAXARGS];
        int arg_num = 0;
        char* arg = strtok(line, " ");
        while (arg && arg_num < MAXARGS - 1) {
            argv[arg_num++] = arg;
            arg = strtok(0, " ");
        }
        argv[arg_num] = NULL;
        char* cmd = argv[0];

        int child_pid = fork();
        if (child_pid == 0) { // in child
            getrusage(RUSAGE_CHILDREN, &rusage_info);
            pageFault_init = rusage_info.ru_majflt;
            pageFaultReclaimed_init = rusage_info.ru_minflt;
            gettimeofday(&clock_init, NULL);
            int grandchild_pid = fork();
            if (grandchild_pid == 0) { // in grandchild
                execvp(cmd, argv);
                fprintf(stderr, "Child process could not do execvp.\n");
                exit(1);
            }
            else { // in child
                wait(NULL);
                getrusage(RUSAGE_CHILDREN, &rusage_info);
                gettimeofday(&clock_end, NULL);
                pageFault_end = rusage_info.ru_majflt;
                pageFaultReclaimed_end = rusage_info.ru_minflt;
                printStatistics(clock_init, clock_end, pageFault_init, pageFault_end, pageFaultReclaimed_init, pageFaultReclaimed_end);
                exit(0);
            }
        }
        else { // in parent
            if (!doneLineNums && currLine == lineNums[lineNums_idx]) { // child backgrounded
                if (lineNums_idx > 0) {
                    background_pids[lineNums_idx] = child_pid;
                }
                lineNums_idx++;
                if (lineNums_idx >= argc-1) {
                    doneLineNums = 1;
                }
            }
            else { // child not backgrounded
                waitpid(child_pid, NULL, 0);
            }
        }
        currLine++;
    }
    wait(NULL);
    for (i = 0; i < argc - 1; i++) {
        waitpid(background_pids[i], NULL, 0);
        exit(1);
    }
}


void main(int argc, char* argv[]) {
    if (argc > 1) {
        handleBackground("multi.txt", argc, argv);
    }
    else {
        handleFile("multi.txt");
    }
}
