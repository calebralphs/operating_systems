# include "boring.h"

// from https://www.zentut.com/c-tutorial/c-read-text-file/
void handleFile(char* filename) {
    FILE* file = fopen(filename, "r");
    char line[MAXCHAR];

    if (file == NULL){
        printf("Could not open file %s", filename);
    }
    else {
        while (fgets(line, sizeof(line), file) != NULL) {
            line[strlen(line)-2] = 0;
            execCommand(line);
        }
    }
}