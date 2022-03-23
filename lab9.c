/* Simple program to illustrate the use of fork-exec-wait pattern. 
 * This version uses execvp and command-line arguments to create a new process.
 * To Compile: gcc -Wall forkexecvp.c
 * To Run: ./a.out <command> [args]
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void sig_usr(int signo) {
	switch(signo) {
	  case SIGINT:
		printf(" - Received SIGINT signal %d\n", signo);
		break;
	  case SIGTSTP:
		printf(" - Received SIGTSTP signal %d\n", signo);
		break;
	  case SIGQUIT:
		printf(" - Received SIGQUIT signal %d\n", signo);
		printf("... Terminating parent process!\n");
		exit(EXIT_SUCCESS); // Terminate the parent process
	  default:
		printf("received signal %d\n", signo);
	}
}

int main(int argc, char **argv) {
    pid_t pid;
    int status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

	// Signal Handlers
	if (signal(SIGINT, sig_usr) == SIG_ERR) {
		printf("can't catch SIGINT\n");
		exit(-1);
	}
	if (signal(SIGTSTP, sig_usr) == SIG_ERR) {
		printf("can't catch SIGTSTP\n");
		exit(-1);
	}
	if (signal(SIGQUIT, sig_usr) == SIG_ERR) {
		printf("can't catch SIGQUIT\n");
		exit(-1);
	}

	// Forking a child process
    pid = fork();
    if (pid == 0) { /* this is child process */
        execvp(argv[1], &argv[1]);
        printf("If you see this statement then execl failed ;-(\n");
	exit(-1);
    } else if (pid > 0) { /* this is the parent process */
        printf("Wait for the child process to terminate\n");
        wait(&status); /* wait for the child process to terminate */

        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else { /* child process did not terminate normally */
            printf("Child process did not terminate normally!\n");
            /* look at the man page for wait (man 2 wait) to determine
               how the child process was terminated */
			if (WIFSIGNALED(status)) {
				printf("Child process exited with signal = %d\n", WTERMSIG(status));
			}
        }
		printf("\n");

		// Parent process continues to wait unitl it receives SIGQUIT
		for ( ; ; ) {
			pause();
		}


    } else { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
    }
    
    printf("[%ld]: Exiting program .....\n", (long)getpid());

    return 0;
}
