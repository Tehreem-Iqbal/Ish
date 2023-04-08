

int getCommand() {
	if(!yyparse()) //return zero on success
		return F_OK;
	else 
		return YYerror;
}

void process(){
	int i;
	/*
	printf("command count: %d\n", commandCount);
	for(i=0;i<commandCount;i++){
		//reset stdin , out, error
		printf("----------------\n");
		printf("command %d : %s\n", i, commandsArray[i]);
			printf("parm count: %d\n",counts[i]);
			for(int p=0; p< counts[i] ;p++)
				printf("command : %d , para %d : %s\n ", i, p, parametersArray[i][p] );
			for(int r=0; r< 5 ;r++){
				switch(redirection[i][r]){
					case NO_REDIRECT:
						printf("NO_REDIRECT\n");
						break;
					case REDIRECT_IN:
						printf("REDIRECT_IN  %s\n", inFile[i]);
						break;
					case REDIRECT_OUT:
						printf("REDIRECT_OUT %s\n", outFile[i]);
						break;
					case REDIRECT_ERROR:
						printf("REDIRECT_ERROR %s\n", errFile[i]);
						break;
					case APPEND:
						printf("APPEND %s\n", outFile[i]);
						break;
					case APPEND_ERROR:
						printf("APPEND_ERROR %s\n", errFile[i]);
						break;
				}
			}
			
		
		printf("-----------------\n");
		
	}
	*/
	for(i=0;i<commandCount;i++){
		reset();
		ioRedirection(i);
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

int executeCommand(int c,char* command, char** args){

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
			execvp(arglist[0], arglist);
			for (int j = 0; j < COMMANDS; j++) 
				free(arglist[j]);
			free(arglist);
			//perror("Command not found.");
			exit(1);
		default:
			if(!background)
				waitpid(cpid, NULL, 0);
			else{
				printf("[%d] %d\n", jobCount+1,cpid);
				saveJob(arglist[0], cpid);
				return 0;
			}
	}	
}
