#include <string.h>
#include <stdio.h>
#include <stdlib.h>  
#include <limits.h>

#include <sys/types.h> 


int* parser(char* x){
  
  char line[100];
  strcpy(line,x);
    
  char *separator = ",";
  char str[16][3];
	int i = 0;
	
	
	char *token = strtok(line, separator);
	do 
	{
		strcpy(str[i++], token);
	}while ((i < 16) && ((token = strtok(NULL, separator)) != NULL));
	
    int y = 0;
    int speed[16];
    char val[3];
    for(y=0; y<=15; y++){
        memcpy(val, str+y, 3);
        speed[y]=atoi(val);
  }
	

