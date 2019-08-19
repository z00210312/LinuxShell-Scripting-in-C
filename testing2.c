/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
// convert all arguments to file describer
// then read or write to the file.


#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>

#define BUFSIZE 128

void testing(char* input[], char* output[], int ac);

int main(int argc, char* argv[])
{
    
    char * argvbuf[20];
    printf("Hello, GDB Online\n");
    
    int i = 0;
    int len, apipe[2];
    char buf[128];
    
    testing(argv, argvbuf, argc);
    
    printf("%s",argvbuf[0]);
    
    /*
    pipe ( apipe );
    len = strlen( argv[0] );
    write( apipe[1], argv[0], len );
    len = read( apipe[0], buf, 128 );
    write( 1, buf, len );
    
    */
    //printf("\n%s : %d",argv[0],len);
    return 0;
}

void testing(char* input[], char* output[], int ac)
{
    int i = 0;
    int j = 0;
    struct stat props;
    int result;
    while(i<ac){
        if (result = stat(input[j], &props) != -1){
            if(S_ISDIR(props.st_mode))
                printf("This is directory : %s\n",input[j]);
            if(S_ISREG(props.st_mode))
                printf("This is regular : %s\n",input[j]);
            if(S_ISLNK(props.st_mode))
                printf("This is link : %s\n",input[j]);
        }
        else{
            printf("error of reading : %s\n",input[j]);
        }
        if(strcmp(input[j],"hello") == 0){
            output[j-1] = input[++i];
        }
        else if(strcmp(input[j],"|") == 0){
            int stdout_copy = dup(1);
            close(1);
            int apipe[2];
            int file1 = creat(apipe[1], 0644 );
            
            j++;
            execlp(input[j],input[j],output[--i]);
            output[++i] = 
            close(file1);
            dup2(stdout_copy, 1);
            close(stdout_copy);
        }
        else{
            output[j] = input[i];
            i++;
            j++;
        }
    }
}



    