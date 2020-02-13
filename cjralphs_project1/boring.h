#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

#define MAXCHAR 128
#define MAXARGS 32

void handleCommand(int pid, char* cmd, char* argv[]) {
    if (pid == 0) {
        execvp(cmd, argv);
        fprintf(stderr, "Child process could not do execvp.\n");
        exit(1);
    }
    else {
        wait(NULL);
    }
}


void printStatistics(struct timeval clock_init,
                     struct timeval clock_end, 
                     long pageFault_init,
                     long pageFault_end,
                     long pageFaultReclaimed_init,
                     long pageFaultReclaimed_end) {
    
    long pageFaults, pageFaultsReclaimed;
    float elapsed_time;

    pageFaults = pageFault_end - pageFault_init;
    pageFaultsReclaimed = pageFaultReclaimed_end - pageFaultReclaimed_init;
    elapsed_time = ((float) clock_end.tv_usec - (float) clock_init.tv_usec)/1000 + ((float) clock_end.tv_sec - (float) clock_init.tv_sec)*1000; 

    printf("\n-- Statistics ---\n");
    printf("Elapsed Time: %.2f milliseconds\n", elapsed_time);
    printf("Page Faults: %ld\n", pageFaults);
    printf("Page Faults (reclaimed): %ld\n", pageFaultsReclaimed);
    printf("-- End of Statistics --\n\n");
}


void execCommand(char* command) {
    struct timeval clock_init;
    struct timeval clock_end;
    struct rusage rusage_info;
    long pageFault_init, pageFault_end, pageFaultReclaimed_init, pageFaultReclaimed_end;

    printf("Running command: %s\n", command);
    command = strdup(command);

    char* argv[MAXARGS];
    int arg_num = 0;
    char* arg = strtok(command, " ");
    while (arg && arg_num < MAXARGS - 1) {
        argv[arg_num++] = arg;
        arg = strtok(0, " ");
    }
    argv[arg_num] = NULL;
    char* cmd = argv[0];

    getrusage(RUSAGE_CHILDREN, &rusage_info);
    pageFault_init = rusage_info.ru_majflt;
    pageFaultReclaimed_init = rusage_info.ru_minflt;
    gettimeofday(&clock_init, NULL);
    
    int pid = fork();
    handleCommand(pid, cmd, argv);

    gettimeofday(&clock_end, NULL);
    getrusage(RUSAGE_CHILDREN, &rusage_info);
    pageFault_end = rusage_info.ru_majflt;
    pageFaultReclaimed_end = rusage_info.ru_minflt;

    printStatistics(clock_init, clock_end, pageFault_init, pageFault_end, pageFaultReclaimed_init, pageFaultReclaimed_end);
}