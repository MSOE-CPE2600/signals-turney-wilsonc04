/**
 * File: recv_signal.c
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * 
 * Brief summary of program:
 * Receives SIGUSR1 signals and prints the sender's PID and attached integer value.
 * Used with send_signal.c to demonstrate interprocess communication using sigqueue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/**
 * @brief Handles incoming SIGUSR1 signals.
 * Prints the sender's process ID and the integer value sent via sigqueue.
 */
static void handle_usr1(int sig, siginfo_t *info, void *ucontext) {
    (void)sig;       // unused parameter
    (void)ucontext;  // unused parameter

    int sender = info ? info->si_pid : -1;          // sender PID
    int val    = info ? info->si_value.sival_int : 0; // value attached to the signal

    printf("Received SIGUSR1 from PID %d with value %d\n", sender, val);
    fflush(stdout);  // ensure output is immediately printed
}

int main(void) {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);       // clear the signal mask
    sa.sa_flags = SA_SIGINFO;       // enable extra info via siginfo_t
    sa.sa_sigaction = handle_usr1;  // assign custom handler

    // Register SIGUSR1 handler
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    // Display process info for sender reference
    printf("Receiver PID: %d\n", getpid());
    printf("Waiting for SIGUSR1 (use send_signal <PID> <value> [count])...\n");

    // Wait indefinitely until a signal is received
    for (;;) pause();  
}
