#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void log_memory_usage(const char *when) {
    char command[256];
    printf("Memory usage %s:\n", when);
    snprintf(command, sizeof(command), "free -h");
    system(command);  // Directly output to console for simplicity
}

int main() {
    int strategies[4] = {0, 1, 2, 3};  // Different strategies for each program
    const char *programs[4] = {"P1.out", "P2.out", "P3.out", "P4.out"};

    printf("Initial system memory state:\n");
    log_memory_usage("before allocations");

    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Child process
            char binaryPath[50];
            snprintf(binaryPath, sizeof(binaryPath), "./bin/%s", programs[i]);
            char strategyArg[10];
            snprintf(strategyArg, sizeof(strategyArg), "%d", strategies[i]);

            // Execute the process binary with the strategy argument
            execl(binaryPath, programs[i], strategyArg, NULL);

            // execl only returns on error
            fprintf(stderr, "Failed to execute %s\n", binaryPath);
            exit(1);
        } else if (pid < 0) {
            // Fork failed
            fprintf(stderr, "Failed to fork for program %s\n", programs[i]);
            exit(1);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    printf("System memory state after all allocations and operations:\n");
    log_memory_usage("after allocations");

    return 0;
}

