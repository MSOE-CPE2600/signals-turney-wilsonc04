/**
 * File: send_signal.c
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * 
 * Brief summary of program:
 * Sends one or more SIGUSR1 signals with an attached integer value
 * to a target process using sigqueue. Used with recv_signal.c to demonstrate
 * interprocess communication with signal data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
    // Validate command-line arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <pid> <value> [count]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse PID, value, and optional count
    pid_t pid = (pid_t)strtol(argv[1], NULL, 10);
    int value = (int)strtol(argv[2], NULL, 10);
    int count = (argc >= 4) ? (int)strtol(argv[3], NULL, 10) : 1;

    // Validate input
    if (pid <= 0 || count <= 0) {
        fprintf(stderr, "Invalid pid or count.\n");
        return EXIT_FAILURE;
    }

    union sigval sv;
    sv.sival_int = value; // value to send with the signal

    // Send the signal multiple times if requested
    for (int i = 0; i < count; ++i) {
        if (sigqueue(pid, SIGUSR1, sv) == -1) {
            perror("sigqueue");
            return EXIT_FAILURE;
        }

        printf("Sent SIGUSR1 to %d with value %d (%d/%d)\n",
               pid, value, i + 1, count);
        fflush(stdout);    // flush output immediately
        usleep(100000);    // small delay (100 ms) for readability
    }

    return EXIT_SUCCESS;
}
