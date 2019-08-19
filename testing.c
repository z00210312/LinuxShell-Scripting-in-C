/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

#define BUFSIZE 128

void testing(char* input[], char* output[]){
    output[0] = input[0];
}

int main(int argc, char* argv[])
{
    
    char * argvbuf[];
    printf("Hello, GDB Online\n");
    
    int i = 0;
    int len, apipe[2];
    char buf[128];
    
    testing(argv, argvbuf);
    
    printf("%s",argvbuf[0]);
    
    /*
    pipe ( apipe );
    len = strlen( argv[0] );
    write( apipe[1], argv[0], len );
    len = read( apipe[0], buf, 128 );
    write( 1, buf, len );
    
    */
    printf("\n%s : %d",argv[0],len);
    return 0;
}