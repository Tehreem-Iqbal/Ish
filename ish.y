%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include "y.tab.h"
#include "ish.h"
#include "redirection.c"
#include "builtin.c"
#include "process.c"
#include "alloc.c"

%}

%union
{
    char	*string;
    int		integer;
}

%token 	<string>	WORD
%token 	<string>	COMMAND
%token 	<string>	FILENAME
%token	<int>		BACKGROUND
%token	<int>		PIPE
%token	<int>		PIPE_ERROR
%token	<int>		SEMICOLON
%token	<int>		REDIRECT_IN
%token	<int>		REDIRECT_OUT
%token	<int>		REDIRECT_ERROR
%token	<int>		APPEND
%token	<int>		APPEND_ERROR
%token	<string>	OPTION
%token	<string>	STRING
%token	<int>		LOGICAL_AND
%token	<int>		LOGICAL_OR
%token	<int>		NO_REDIRECT
%%

cmd_line 	: cmd_line separator COMMAND parameters { strcpy(commandsArray[commandCount++], $3);  }
        | COMMAND parameters  { strcpy(commandsArray[commandCount++], $1);  }
		| cmd_line BACKGROUND {  background = true;  }
		| cmd_line SEMICOLON
		|  
		| error 
		;

separator 	: BACKGROUND  { printf("bg"); background = true;}
		| PIPE { printf("pipe"); 
		counts[commandCount-1] = paramCount; paramCount = 0;  parameterCount = commandCount;iter_red = 0;
		isPipe[pipeCount++] = PIPE;
		}
		| PIPE_ERROR { printf("pipe_error"); 
		counts[commandCount-1] = paramCount; paramCount = 0;  parameterCount = commandCount;iter_red = 0;
		isPipe[pipeCount++] = PIPE_ERROR;
		}
		| SEMICOLON {   counts[commandCount-1] = paramCount; paramCount = 0;  parameterCount = commandCount;iter_red = 0;}
		;

parameters	: parameters OPTION { strcpy(parametersArray[parameterCount][paramCount++], $2); }
		| parameters STRING { strcpy(parametersArray[parameterCount][paramCount++], $2); }
		| parameters WORD { strcpy(parametersArray[parameterCount][paramCount++], $2); }
		| parameters REDIRECT_IN FILENAME { redirection[commandCount][iter_red++] = REDIRECT_IN; strcpy(inFile[commandCount] ,$3); }
        | parameters REDIRECT_OUT FILENAME { redirection[commandCount][iter_red++] = REDIRECT_OUT; strcpy(outFile[commandCount] , $3); }
		| parameters REDIRECT_ERROR FILENAME { redirection[commandCount][iter_red++] = REDIRECT_ERROR; strcpy(errFile[commandCount] , $3);}
		| parameters APPEND FILENAME { redirection[commandCount][iter_red++] = APPEND; strcpy(outFile[commandCount],$3); }
		| parameters APPEND_ERROR FILENAME { redirection[commandCount][iter_red++] = APPEND_ERROR; strcpy(outFile[commandCount] , $3); }
		|
		;

%%

int yyerror(char *s){
    fprintf(stderr, "syntax error\n");
    return 0;
}



int main(){
	//signal(SIGCHLD, sigchild_handler);
	jobCount = 0;
	jobs = malloc(sizeof(struct Job)*JOBS);
	while(1){		
		init();
        switch(getCommand()) {
			case YYerror:
				break;
			case F_OK:
				counts[commandCount-1] = paramCount;	
				process();
				destroy();
				break;
			default: 			
				break;
		}
	}
	printf("\n");
	return 0;
}

