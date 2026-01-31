#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>


void handle_sigchld(int sig) {
    int saved_errno = errno; 
    pid_t pid;
    int status;
      

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Reaped zombie process %d\n", pid);
    }
   
    errno = saved_errno; 
}

int main() {
    struct sigaction sa;
        

    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; 
   
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
          perror("sigaction");
          exit(EXIT_FAILURE);
    }	    

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
	    perror("sigaction for SIGHUP");
	    exit(EXIT_FAILURE);
    }

    while (1) {

           sleep(10);	            
      }
	        
    return 0;
}
