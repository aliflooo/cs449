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

char *delims = " ()<>|&;\t\n";	// Set delimiters as global variables

int get_num_arguments(char* input) { // This method will get the number of arguments
  char arguments[300];	
  char* curr;
  int count = 0;

  strcpy(arguments, input);

  curr = strtok(arguments, delims);
  while(curr != NULL){
    count = count + 1;
    curr = strtok(NULL, delims);
  }
  return count;
}

int main() {
  char** args_array;
  char* arg;
  char input[300];
  char token[300];
  int childpid = 0;
  int args = 0;
  int status;
  int count = 0;
  int x;

  signal(SIGINT, SIG_IGN); // Ignores signal for now

  while(1) {
    printf("myshell> ");
    fgets(input, 300, stdin); // Allocate space for an input buffer of 300 characters

    args = get_num_arguments(input);
    args_array = malloc(sizeof(char*) * (args + 1));

    strcpy(token, input);

    arg = strtok(token, delims);
    count = 0;
    while(arg != NULL){ // This will tokenize the string given
      args_array[count] = (char*) malloc(sizeof(char) * strlen(arg));
      strcpy(args_array[count], arg);

      arg = strtok(NULL, delims);
      count++;
    }
    args_array[args] = NULL;

    if(args_array[0] == NULL){
      continue;
    }

	if(!strcmp(args_array[0], "exit") && (args_array[1] != NULL)) { // If the user wishes to exit, it will check if there is an exit number
		arg = strtok(NULL, " "); // Tokenizes by an empty space
		sscanf(args_array[1], "%d", &x);
		return x; // Return the exit code
	}

	else if(!strcmp(args_array[0], "exit")) { // If the command is just exit, it will exit out of shell
    	exit(0);
    }

    if(strcmp(args_array[0], "cd") == 0){ // If the command is cd, it will  check to see if the directory exists or not
      if(chdir(args_array[1]) == -1){
        if(args_array[1] == NULL){
          printf("cd: : Please pick a directory\n"); // If the user enters NULL, will ask to specify the directory
        }
        else {
          printf("cd: %s: Directory or file does not exist\n", args_array[1]); // Make sure the directory exists
        }
      }
    }

    else {
      if(fork() != 0){ // Same as Lab 7; will check if there are errors and executes proper exit/termination
        childpid = waitpid(-1, &status, 0);

        if(childpid == -1)
		{
			perror("Error running program");
		}
		else if(status == 0) {
			//printf("Program exited successfully!\n");
		}
		else if(WEXITSTATUS(status))
		{
			printf("\nExited successfully with exit code %d\n", WEXITSTATUS(status));
		}
		else if(signal(SIGINT, SIG_ERR))
		{
			printf("\n");
			printf("Program was terminated by signal: %s\n", strsignal(status));
		}
		else
		{
			printf("Terminated some other way!\n");
		}
      }

      else { // This will work input and output redirection
        if(strstr(input, ">>") != NULL){
          freopen(args_array[args-1], "a", stdout);

          free(args_array[args-1]);
          args_array[args-1] = NULL;
        }

        else if(strstr(input, ">") != NULL){
          freopen(args_array[args-1], "w", stdout);

          free(args_array[args-1]);
          args_array[args-1] = NULL;
        }

        else if(strstr(input, "<") != NULL){
          freopen(args_array[args-1], "r", stdin);

          free(args_array[args-1]);
          args_array[args-1] = NULL;
        }

        signal(SIGINT, SIG_DFL);
        execvp(args_array[0], args_array);
        exit(0);
      }
    }
  }
  return 0;
}