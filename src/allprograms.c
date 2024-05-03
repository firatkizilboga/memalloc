#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_PROGRAMS 4

int main() {
    const char *programs[MAX_PROGRAMS] = {"P1.o", "P2.o", "P3.o", "P4.o"};
    char strategy[10]; // Buffer to hold strategy input
    char binaryPath[256]; // Buffer to hold the path to the binary
    char cwd[256]; // Buffer to store the current working directory

    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    // Prompt user for the strategy number
    printf("Enter the memory allocation strategy number (0-3): ");
    if (fgets(strategy, sizeof(strategy), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    strategy[strcspn(strategy, "\n")] = 0; // Remove newline character

    // Iterate over each program and run it sequentially
    for (int i = 0; i < MAX_PROGRAMS; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Child process
            snprintf(binaryPath, sizeof(binaryPath), "%s/bin/%s", cwd, programs[i]);
            char *args[] = {binaryPath, strategy, NULL};

            // Redirect child's output to stdout
            dup2(STDOUT_FILENO, 1);
            printf("\nExecuting %s with strategy %s\n", programs[i], strategy);
            // Execute the program binary with strategy argument
            execvp(binaryPath, args);

            // execvp only returns on error
            fprintf(stderr, "Failed to execute %s\n", binaryPath);
            exit(1);
        } else if (pid > 0) { // Parent process
            int status;
            waitpid(pid, &status, 0); // Wait for the child to complete

            if (WIFEXITED(status)) {
                printf("Program %s (PID: %d) completed with exit status %d\n", programs[i], pid, WEXITSTATUS(status));
            }
        } else {
            // Fork failed
            fprintf(stderr, "Failed to fork for program %s\n", programs[i]);
            return 1;
        }
    }

    return 0;
}

