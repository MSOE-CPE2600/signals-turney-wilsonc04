/**
 * @file signal_segfault.c
 * @brief Deliberately triggers a segmentation fault and installs a SIGSEGV handler that prints and returns.
 *
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * Brief summary of modifications:
 *  - Creates a NULL pointer and dereferences it to cause SIGSEGV.
 *  - SIGSEGV handler prints a message and returns (no recovery).
 *  - Program behavior: it re-enters the same faulting instruction and signals again.
 */

#include <stdio.h>
#include <signal.h>

static void handle_segv() {
    printf("Handler: caught SIGSEGV (segmentation fault). Returning...\n");
    // Per the assignment, do not exit or modify control flow here.
}

int main(void) {
    signal(SIGSEGV, handle_segv);

    // Deliberately cause a segmentation fault
    int *i = NULL;
    printf("About to dereference a NULL pointer...\n");
    printf("Value at *i: %d\n", *i);  // faulting instruction

    // Unreachable if fault repeats
    printf("If you see this, the fault did not reoccur.\n");
    return 0;
}
