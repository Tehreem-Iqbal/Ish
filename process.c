// Parse command line input
int getCommand() {
	if(!yyparse()) //return zero on success
		return F_OK;
	else 
		return YYerror;
}

// Execute builtin command and pass control to executeCommand() for external commands
void process(){
	int i;
	for(i=0;i<commandCount;i++){
		reset(); // reset standard input and output
		ioRedirection(i); // redirect input and output
		if(isBuiltin(commandsArray[i])) {			
			if(!strcmp(commandsArray[i],"cd"))	
				cd(parametersArray[i][0], i);
			else if(!strcmp(commandsArray[i],"exit"))	
				bye();
			else if(!strcmp(commandsArray[i],"setenv"))	
				setEnv(parametersArray[i][0], parametersArray[i][1], i);
			else if(!strcmp(commandsArray[i],"unsetenv"))	
				unsetEnv(parametersArray[i][0]);
			else if(!strcmp(commandsArray[i],"jobs"))	
				printJobs();
			else if(!strcmp(commandsArray[i],"kill"))	
				killJob(parametersArray[i][0]);
			else if(!strcmp(commandsArray[i],"bg"))	
				bg(parametersArray[i][0]);
			else if(!strcmp(commandsArray[i],"fg"))	
				fg(parametersArray[i][0]);
			
		}			
		else{
			executeCommand(i, commandsArray[i], parametersArray[i]);
		}
	}
}

// Execute external commands
int executeCommand(int c,char* command, char** args){
	int rv;
	arglist = (char**)malloc(COMMANDS*sizeof(char*));
	for(int j=0; j < COMMANDS; j++){
		arglist[j] = (char*)malloc(sizeof(char)* COMMANDS);
		bzero(arglist[j],COMMANDS);
	}
	strcpy(arglist[0], command);

	int i;
	for(i=0 ;i<counts[c];i++)
		strcpy(arglist[i+1], args[i]);		
	
	arglist[i+1] = NULL;
	int cpid = fork();
	switch(cpid){
		case -1:
		perror("fork failed");
				exit(1);
		case 0:
			rv = execvp(arglist[0], arglist);
			if(rv == -1){
				printf("Command not found\n");
			}
			for (int j = 0; j < COMMANDS; j++) 
				free(arglist[j]);
			free(arglist);	
			exit(1);
		default:
			if(!background) 
				waitpid(cpid, NULL, 0); // wait for child process to terminate
			else{
				printf("[%d] %d\n", jobCount+1,cpid); // print job id and pid for background process
				saveJob(arglist[0], cpid); // save job in jobs array
				return 0;
			}
	}	
}
