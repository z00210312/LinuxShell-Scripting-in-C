// This program is originally created by instructor
// Modified by Jia Shin Tseng
// Modified date 5/22/2017
// Assignment-08
/*
Write your own simple shell.  Your shell should prompt the user for a command, run it, then prompt the user for their next command.  If the user types "exit", then the shell should terminate.  The shell should ignore Ctrl-C.

Hint:
Remember the logic for the shell :

while input != exit
    parse the command
    fork
    if parent process :
        wait
    else
        execvp command

You'll need the chdir system call to add support for cd.  This needs to be part of the shell as their is no cd system command.

man chdir, execvp, wait, fork, strtok

This may be the most challenging assignment of the quarter for some people.  Don't wait to get started on it.
*/

#include <unistd.h>
#include <wait.h>   // contains macros for testing information returned by wait
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "parse.h"

void run_child_code(char*[]);
void run_parent_code(char*);

int main(int ac, char * av[]){
	signal(SIGINT, SIG_IGN);
	while(1){
        int fork_rv = 0;
        char * charbuf = (char*)malloc(1);
        const int MAX_ARRY_VALUE = 255;
        char * inputToken[MAX_ARRY_VALUE];
        
    	*charbuf = 10;
        while(*charbuf == 10){
        	if(read(STDIN_FILENO, charbuf, MAX_ARRY_VALUE) == -1){
        		perror("read() error");
        		break;
        	}
		}
		
		parse(charbuf,inputToken);
		if(strcmp(inputToken[0],"cd") == 0)
		    if(chdir(inputToken[1]) == -1)
				perror("chdir() error");
		if(strcmp(inputToken[0],"exit") == 0)
		    break;
		
		fork_rv = fork();
		if (fork_rv == 0)
			run_child_code(inputToken);
		else
			run_parent_code(inputToken[0]);
		
        free(charbuf);
        charbuf = (char*)malloc(1);
	}
	return 0;
}

void run_child_code(char* argToken[]){
	execvp( argToken[0], argToken );
	exit(2);
}

void run_parent_code(char* inputChar){
    int wait_exit_value;
    int wait_rv;
    wait_rv = wait( &wait_exit_value );
	if(wait_exit_value>>8 == 2 && strcmp(inputChar,"cd") != 0)
		write(0,"No such commend\n",sizeof("No such commend\n"));
}