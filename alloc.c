
// Signal handler for SIGCHLD
void sigchild_handler(int signum){
	int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
			for(int i=0;i<jobCount;i++){
				if(jobs[i].jobpid == pid){
					jobs[i].jobstatus = "Done";
					printf("\n %s %s \n", jobs[i].jobname, jobs[i].jobstatus );
					break;
				}
			}
          
        }
		 else if (WIFSIGNALED(status)) {
			for(int i=0;i<jobCount;i++){
				if(jobs[i].jobpid == pid){
					jobs[i].jobstatus = "Stopped";
					printf("\n%s %s \n", jobs[i].jobname, jobs[i].jobstatus );
					break;
				}
			}
        }
    }
}

//Signal handler for SIGTERM
void termHandler(int sig) {
    printf("Received TERM signal\n");
    exit(0);
}

// Get hostname
char* hostname(){
	char *hostbuffer = (char*)malloc(sizeof(char)* PATH);
	gethostname(hostbuffer, PATH);
	return hostbuffer;
}
// Initialize variables and memory
void init(){
	int i, j;
	printf("\n");
	printf("[1;34;40m" "%s%% ", hostname());
	reset();
	//reset variables
	commandCount = 0;
	parameterCount = 0;
	paramCount = 0;
	background = false;
	iter_red = 0;
	pipeCount = 0;

	commandsArray = (char**) malloc(COMMANDS * sizeof(char*));
	for(j=0; j < COMMANDS; j++){
		commandsArray[j] = (char*)malloc(sizeof(char)* ARGLEN);
		bzero(commandsArray[j],ARGLEN);	
	}
	
	parametersArray = (char***) malloc(COMMANDS * sizeof(char**));
	for(i=0; i < COMMANDS; i++){
		parametersArray[i] = (char**)malloc(sizeof(char*)* PARAMETERS);
		for (int p=0; p< PARAMETERS;p++){
			parametersArray[i][p] = (char*)malloc(sizeof(char)* ARGLEN);
			memset(parametersArray[i][p], 0,ARGLEN );
		}
	}
	

	//Redirection files
	inFile = (char**)malloc(sizeof(char*)*PARAMETERS);
	for(j=0; j < PARAMETERS; j++){
		inFile[j] = (char*)malloc(sizeof(char)* PARAMETERS);
		bzero(inFile[j],PARAMETERS);	
	}
	outFile = (char**)malloc(sizeof(char*)*PARAMETERS);
	for(j=0; j < PARAMETERS; j++){
		outFile[j] = (char*)malloc(sizeof(char)* PARAMETERS);
		bzero(outFile[j],PARAMETERS);	
	}
	errFile = (char**)malloc(sizeof(char*)*PARAMETERS);
	for(j=0; j < PARAMETERS; j++){
		errFile[j] = (char*)malloc(sizeof(char)* PARAMETERS);
		bzero(errFile[j],PARAMETERS);	
	}

	//redirection type
	for(i = 0; i < COMMANDS; i++){
		redirection[i] = (int*)malloc(5 * sizeof(int));
		for(int j = 0; j < 5; j++){
			redirection[i][j] = NO_REDIRECT;
		}
	}
	//Saving std file descriptors
	dp_in = dup(0);
	dp_out = dup(1);
	dp_err = dup(2);

	//pipes
	for(i=0;i<COMMANDS-1;i++){
		isPipe[i] = 0;
	}
	// Ignore QUIT signal
	signal(SIGQUIT, SIG_IGN); 
	signal(SIGINT, SIG_IGN); 
	signal(SIGTERM, termHandler);	
	signal(SIGCHLD, sigchild_handler);
}

// Free memory
void destroy(){
	for (int j = 0; j < COMMANDS; j++) 
		free(commandsArray[j]);	
	free(commandsArray);
	
	for (int i = 0; i < COMMANDS; i++) {
		for (int p = 0; p < PARAMETERS; p++) {			
			free(parametersArray[i][p]);
		}
		free(parametersArray[i]);
	}
	free(parametersArray);
	//i want to free the double pointerfiles inFile and outFile
	for (int j = 0; j < PARAMETERS; j++) {
		free(outFile[j]);
		free(inFile[j]);
		free(errFile[j]);	
	}
	for(int i=0; i<5; i++){
		free(redirection[i]);
	}
	free(inFile);
	free(outFile);
	free(errFile);
	
}
