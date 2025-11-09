/**
 * @file signal_handler.c
 * @brief Registers a signal handler for SIGINT. The handler prints a message but does NOT exit.
 *
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * Brief summary of modifications:
 * - Removed the call to exit() inside the signal handler.
 * - Added a message to indicate the program continues running after SIGINT.
 * - The program now only exits when SIGKILL is sent manually.
 */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Signal handler for SIGINT - prints a message but does not exit
 */
void handle_signal() {
    printf("\nReceived SIGINT (Ctrl+C), but not exiting.\n");
}

/**
 * @brief Main function that registers the signal handler and loops until killed
 */
int main() {
    // Register the signal handler for SIGINT
    signal(SIGINT, handle_signal);

    printf("Process PID: %d\n", getpid());
    printf("Press Ctrl+C to send SIGINT. Use 'kill -9 %d' to stop the process.\n", getpid());

    // Infinite loop until killed
    while (1) {
        printf("Sleeping...\n");
        sleep(1);
    }

    return 0;
}