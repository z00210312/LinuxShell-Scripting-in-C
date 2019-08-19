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

int main(int argc, char* argv[])
{
	
	int stdoutcpy = dup(1);
	int fd[2];
	pipe(fd);
	int filedata;
	int i = 0;
	while(argv[i]){
		if(strcmp(argv[i],"|") == 0)
			break;
		i++;
	}
	
	printf("error0 : %d\n",i);
	if(argc>0){
		int fork_rv = fork();
		if (fork_rv == 0){
			int charPos = i + 1;
			int commPos = i - 1;
			printf("error1 : %d\n",charPos);
			close(1);
			dup2(fd[1],1);
			if(execlp(argv[charPos],argv[charPos],argv[commPos],NULL) == -1)
				if(execlp(argv[charPos],argv[charPos],argv[commPos-1],argv[commPos+1],NULL) == -1)
					exit(17);
		}
		else{
			run_parent_code();
			write(fd[1],"\n",sizeof("\n"));
			close(fd[1]);
			dup2(stdoutcpy,1);
			printf("error2\n");
			close(1);
			//dup2(fd[1],1);
			filedata = creat( "doctext", 0644 );
			char * stringbuf = (char*)malloc(1);
			char * charbuf = (char*)malloc(1);
			char * inputToken[255];
			read(fd[0],stringbuf,500);
			//execlp("wc", "wc",fd,NULL);
			//printf("error\n");
			parse(stringbuf,inputToken);
			
			int i = 0;
			printf("%s",inputToken[i++]);
			while(inputToken[i] != 0){
				printf(" %s",inputToken[i++]);
			}
		}
	}
	return 0;
}