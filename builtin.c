
bool isBuiltin(char* command){

	if(!strcmp(command,"cd") || !strcmp(command,"jobs") || !strcmp(command,"bg")|| !strcmp(command,"fg") || !strcmp(command,"kill") || !strcmp(command,"setenv") || !strcmp(command,"unsetenv"))
		return true;
	return false;
}

int cd(char* arg, int cn){
	int cd_rv;

	if(counts[cn] == 0){
		char *home = getenv("HOME");
    	cd_rv = chdir(home);
	}
	else{
	cd_rv = chdir(arg);
	}
	if (cd_rv < 0){
		perror("cd ");
		return 0;
	}
}

void bye(){	
	printf("[01;31;40m" "\n********  BYE  **********\n");
	printf("[00;00;00m");
	exit(0); //exit shell
		
}

void printEnv(){
    char **env = environ;
    // Loop through each environment variable and print its name and value
    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
}

void setEnv(char* var, char* value, int cn){
	if(counts[cn] == 0){
		printEnv();
	}
	else{
	setenv(var, value, 0);
	char *val = getenv(var);
	printf("Environment variable set\n %s = %s\n", var, val);
	}
}
void unsetEnv(char* var){
	unsetenv(var);
}

// jobs

void killJob(char *id){
	int jobid = atoi(id+1);
	int i;
	for(i=0; i<jobCount; i++){
		if(jobs[i].jobid == jobid){
			kill(jobs[i].jobpid, SIGTERM );
			jobs[i].jobstatus = "TERMINATED";
			printf("[%d] \t\t  Job terminated\n", jobid);
			break;
		}
	}
	if(i==jobCount)
		printf("Kill: Job not found\n");
}

void bg(char *id){
	int jobid = atoi(id+1);
	int i;
	for(i=0; i<jobCount; i++){
		if(jobs[i].jobid == jobid){
			kill(jobs[i].jobpid, SIGCONT );
			jobs[i].jobstatus = "TERMINATED";
			printf("[%d] \t\t  Job terminated\n", jobid);
			break;
		}
	}
	if(i==jobCount)
		printf("bg: Job not found \n");
}

void fg(char *id){
	int jobid = atoi(id+1);
	int i;
	for(i=0; i<jobCount; i++){
		if(jobs[i].jobid == jobid){
			kill(jobs[i].jobpid, SIGCONT );
			printf("%s\n",jobs[i].jobname);
			waitpid(jobs[i].jobpid, NULL, 0);		
			break;
		}
	}
	if(i==jobCount)
		printf("fg: Job not found \n");
}


void saveJob(char *com, int pid){
	jobs[jobCount].jobid = jobCount+1;
	jobs[jobCount].jobpid = pid;
	jobs[jobCount].jobstatus = "RUNNING";
	jobs[jobCount].jobname =  com;
	jobCount++;
			
}

void printJobs(){
	for(int i=0; i<jobCount; i++)
		printf("[%d] %s \t %s\n", jobs[i].jobid, jobs[i].jobstatus , jobs[i].jobname);
}