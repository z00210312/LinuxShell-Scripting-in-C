/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "fcntl.h"

void run_parent_code(){
    int wait_exit_value;
    int wait_rv;
    wait_rv = wait( &wait_exit_value );
}

void parse( char* input, char* args[] )
{
    int i = 0;
    // fgets reads the \n, so overwrite it
    input[strlen(input)-1] = '\0';  
    // get the first token
    args[i] = strtok( input, " " );
    // get the rest of them
    while( ( args[++i] = strtok(NULL, " ") ) );
}

int main()
{
    int fd[2];
    pipe(fd);
    int fork_rv = fork();
    if (fork_rv == 0){
        dup2(fd[1],1);
        execlp("ls","ls -i",NULL);
    }
    else{
        run_parent_code();
        //write(fd[1],"\n",sizeof("\n"));
        write(fd[1],"testing",sizeof("testing"));
        write(fd[1],"\0",sizeof("\0"));
        char * stringbuf = (char*)malloc(1);
        char * charbuf = (char*)malloc(1);
        char * inputToken[255];
        read(fd[0],stringbuf,100);
        parse(stringbuf,inputToken);
        //write(stdout,stringbuf,len);
        int i = 0;
        while(inputToken[i] != 0){
            printf(inputToken[i++]);
        }
    }
}