#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    FILE *genesis = fopen("genesis.txt", "r");

    // Get the size of genesis.txt
    fseek(genesis, 0, SEEK_END);
    int count = ftell(genesis);
    fclose(genesis);

    int n = 4; // Number of processes
    int length = count / n;

    for (int i = 0; i < n; i++) {
        int pid = fork();

        if (pid == 0) { // Child process
            FILE *file = fopen("genesis.txt", "r");
            if (file == NULL) {
                exit(1);
            }

            char *buffer = malloc(length);
            if (buffer == NULL) {
                fclose(file);
                exit(1);
            }

            fseek(file, i * length, SEEK_SET);
            fread(buffer, length, 1, file);
            fclose(file);

            char filename[30];
            sprintf(filename, "genesis_part_%d.txt", i + 1);

            FILE *newFile = fopen(filename, "w");
            if (newFile == NULL) {
                free(buffer);
                exit(1);
            }

            fwrite(buffer, length, 1, newFile);
            fclose(newFile);
            free(buffer);

            exit(0);
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
