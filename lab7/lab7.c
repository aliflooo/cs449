// Aliyah Flores
// aff13

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	if(argc < 2){
		printf("Please provide an executable to run.\n");
		return 0;
	}

	if(fork() == 0)
	{
		execvp(argv[1], &argv[1]);

		perror("Error running program");
		exit(1);
	}
	else
	{
		signal(SIGINT, SIG_IGN);

		int status;
		int childpid = waitpid(-1, &status, 0);
		printf("----------\n");

		if(childpid == -1)
		{
			perror("Error running program");
		}
		else if(status == 0) {
			printf("Program exited successfully!\n");
		}
		else if(WIFEXITED(status))
		{
			printf("Exited successfully with exit code %d\n", WEXITSTATUS(status));
		}
		else if(signal(SIGINT, SIG_ERR))
		{
			printf("Program was terminated by signal: %s\n", strsignal(status));
		}
		else
		{
			printf("Terminated some other way!");
		}
	}
	return 0;
}