/**
 * @file signal_sigaction.c
 * @brief Demonstrates use of sigaction to handle SIGUSR1 and display sender PID.
 *
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * Brief summary of program:
 *  - Registers a SIGUSR1 handler using sigaction with SA_SIGINFO flag.
 *  - Handler prints the sender's process ID (si_pid).
 *  - Program then loops forever waiting for signals.
 *
 * To send SIGUSR1 from another terminal:
 *   kill -SIGUSR1 <PID>
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/**
 * @brief Handler for SIGUSR1 using sigaction
 * @param sig  The received signal number
 * @param info Pointer to siginfo_t with details about the signal
 * @param context Unused pointer to ucontext_t structure
 */
void handle_sigusr1(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;
    printf("Received SIGUSR1 from process %d\n", info->si_pid);
}

int main(void) {
    struct sigaction sa;

    // Clear the sigaction structure
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;         // Enable additional info in handler
    sa.sa_sigaction = handle_sigusr1; // Assign handler function

    // Register handler for SIGUSR1
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
    printf("Waiting for SIGUSR1...\n");
    printf("Send using: kill -SIGUSR1 %d\n", getpid());

    // Loop indefinitely until manually terminated
    while (1) {
        pause(); // wait for signal
    }

    return 0;
}
