// splits input into separate strings
// inputs:  input: a string to split up, args: an array of char pointers to
//    store the separate strings
// outputs: an array of strings, with null in the last element, passed back
//    through args
// preconditon: input is a valid c-string, read from the keyboard using fgets
// postcondition:  args contains the separate strings, one 
//    string per element.  Last element contains null
#ifndef PARSE_H
#define PARSE_H

#include "string.h"

static void parse( char* input, char* args[] )
{
  int i = 0;
  
  // fgets reads the \n, so overwrite it
  input[strlen(input)-1] = '\0';  
  
  // get the first token
  args[i] = strtok( input, " " );
  
  // get the rest of them
  while( ( args[++i] = strtok(NULL, " ") ) );
}

#endif