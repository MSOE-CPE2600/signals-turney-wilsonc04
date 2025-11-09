/**
 * File: pickle_recv.c
 * Modified by: Caleb Wilson
 * CPE 2600 111
 * 
 * Brief summary of program: Receiver program for signal-based pickleball.
 * Waits for the first serve, exchanges SIGUSR1 signals with the server,
 * tracks scoring per pickleball rules, and exits cleanly when the match ends.
 */

// Build: gcc -o p_recv pickle_recv.c
// Run:   ./p_recv

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

static pid_t my_pid, peer_pid = -1, server_pid = -1;
static int my_score = 0, opp_score = 0;
static volatile sig_atomic_t rally_active = 0;

static void rand_delay(void){ usleep(1000000 + (rand()%1000001)); } // 1–2s
static void bell_send(int count){
    union sigval sv; sv.sival_int = count;
    sigqueue(peer_pid, SIGUSR1, sv);
    printf("\a[RECV %d] send (count=%d) -> %d\n",(int)my_pid,count,(int)peer_pid);
    fflush(stdout);
}
static int game_over(void){
    return ((my_score>=11 || opp_score>=11) && abs(my_score-opp_score)>=2);
}
static void announce_and_end(const char* who){
    printf("[RECV %d] %s | FINAL: me %d - opp %d\n",
           (int)my_pid, who, my_score, opp_score);
    // tell peer to exit too
    sigqueue(peer_pid, SIGUSR2, (union sigval){.sival_int=0});
    exit(0);
}
static void try_serve_if_server(void){
    if(!rally_active && server_pid==my_pid){
        rally_active=1; rand_delay(); bell_send(1);
    }
}

static void on_gameover(int s){ (void)s;
    printf("[RECV %d] game over signal received. FINAL: me %d - opp %d\n",
           (int)my_pid, my_score, opp_score);
    exit(0);
}

static void on_ball(int s, siginfo_t* info, void* u){ (void)s; (void)u;
    if(peer_pid==-1 && info) peer_pid=info->si_pid;
    int count = info? info->si_value.sival_int : 0;
    printf("[RECV %d] recv from %d (count=%d)  server=%d  score %d-%d\n",
           (int)my_pid, info?(int)info->si_pid:-1, count, (int)server_pid, my_score, opp_score);

    if(count==0){
        // sender missed; rally ends
        rally_active=0;
        if(server_pid==my_pid){
            my_score++;                     // only server scores
            if(game_over()) announce_and_end("I won the match");
            // server keeps serve
        }else{
            server_pid=my_pid;              // server lost -> switch serve
        }
        try_serve_if_server();
        return;
    }

    // keep rally going
    rand_delay();
    if(rand()%2==0) bell_send(count+1);     // good return
    else            bell_send(0);           // I miss -> sender scores if they were server
}

int main(void){
    my_pid=getpid();
    srand((unsigned)time(NULL) ^ (unsigned)my_pid);

    struct sigaction sa={0}; sigemptyset(&sa.sa_mask);
    sa.sa_flags=SA_SIGINFO; sa.sa_sigaction=on_ball;
    if(sigaction(SIGUSR1,&sa,NULL)==-1){perror("sigaction");return 1;}

    struct sigaction so={0}; so.sa_handler=on_gameover; sigemptyset(&so.sa_mask);
    if(sigaction(SIGUSR2,&so,NULL)==-1){perror("sigaction");return 1;}

    server_pid=-1; // server program starts as server
    printf("Receiver PID: %d  (run server with this PID)\n",(int)my_pid);
    for(;;) pause();
}
