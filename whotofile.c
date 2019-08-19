/* whotofile.c
 *      purpose:  show how to redirect output for another program
 *      idea:     fork, then in the child, redirect output, then exec
 */

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "fcntl.h"


int main(int argc, char* argv[])
{
    int pid, fd;

    printf( "About to run who into a file\n" );

    // create a new process or quit
    if( ( pid = fork() ) == -1 )
    {
        perror( "fork" );
        exit(1);
    }

    // child does the work
    if( pid == 0 )
    {
        close(1);                                   // close
        fd = creat( "doctext", 0644 );             // then open
		switch(argc){
			case 1:
			case 2:
				execlp(argv[1],argv[1],NULL);
				break;
			case 3:
				execlp(argv[1],argv[1],argv[2],NULL);
				break;
			case 4:
				execlp(argv[1],argv[1],argv[2],argv[3],NULL);
				break;
			case 5:
			case 6:
			case 7:
			default:
			break;
		}
		//execlp(argv[1],argv[1],argv[2],NULL);
        //execlp( argv[1], argv[1], NULL );               // and run
        perror( "execlp" );
        exit(1);
    }

    // parent waits then reports
    if( pid != 0 )
    {
        wait( NULL );
        printf( "Done running who.  results in userlist\n" );
    }

    return 0;
}
