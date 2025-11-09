/**
 * File: signal_alarm.c
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * Brief summary of program:
 *  - Includes <signal.h> for signal-related declarations (SIGALRM, signal()).
 *  - Calls alarm(5) to schedule SIGALRM.
 *  - Prints message when SIGALRM is received.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/**
 * @brief Signal handler for SIGALRM
 */
static void handle_alarm()
{
    printf("Received SIGALRM for 5 seconds\n");
}

int main(void)
{
    signal(SIGALRM, handle_alarm);

    printf("Process PID: %d\n", getpid());
    printf("Scheduling Alarm for 5 seconds\n");
    alarm(5);

    // Wait indefinitely for signals
    while(1)
    {
        pause(); // suspend until a signal is caught
    }
    return 0;
}
