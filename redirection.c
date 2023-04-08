void ioRedirection(int commandNo){
	for(int r=0; r< 5 ;r++){
		switch(redirection[commandNo][r]){
			case NO_REDIRECT:
				//printf("NO_REDIRECT\n");
				break;
			case REDIRECT_IN:
				//printf("REDIRECT_IN  %s\n", inFile[commandNo]);
				redirectInput(inFile[commandNo]);
				break;
			case REDIRECT_OUT:
				//printf("REDIRECT_OUT %s\n", outFile[commandNo]);
				redirectOutput(outFile[commandNo]);
				break;
			case REDIRECT_ERROR:
				//printf("REDIRECT_ERROR %s\n", errFile[commandNo]);
				redirectError(errFile[commandNo]);
				break;
			case APPEND:
				//printf("APPEND %s\n", outFile[commandNo]);
				redirectOutAppend(outFile[commandNo]);
				break;
			case APPEND_ERROR:
				//printf("APPEND_ERROR %s\n", errFile[commandNo]);
				redirectErrAppend(errFile[commandNo]);
				break;
		}
	}
}

// < Redirect the standard input.
void redirectInput(char* inFile){
    int fd_in = open(inFile, O_RDONLY, 0);
    if(fd_in == -1) perror("File Opening Error\n");
    else dup2(fd_in,0);
	
}
// >
// Redirect the standard output to a file. If the file does not exist, it is created.
void redirectOutput(char* outFile){
    int fd_out = open(outFile, O_WRONLY | O_TRUNC | O_CREAT , 0777);
    if(fd_out == -1) perror("File Opening Error\n");
    else dup2(fd_out,1);
}


// >& 
// //The ’&’ form redirects both standard output and the standard error to the file.
void redirectError(char* errFile){  
    int fd_err = open(errFile, O_WRONLY | O_TRUNC | O_CREAT , 0777);
    if(fd_err == -1) perror("File Opening Error\n");
    else 
	{
	dup2(fd_err,1);
    dup2(fd_err,2);
	}
}

// >>
// Append the standard output. Like ’>’, but places output at the end of the file rather than overwriting it.
void redirectOutAppend(char* outFile){
    int fd_out = open(outFile, O_WRONLY | O_APPEND | O_CREAT , 0777);
    if(fd_out == -1) perror("File Opening Error\n");
    else dup2(fd_out,1);
}
// >>& 
//  The ’&’ form appends both the standard error and standard output to the file.
void redirectErrAppend(char* errFile){
    int fd_err = open(errFile, O_WRONLY | O_APPEND | O_CREAT , 0777);
    if(fd_err == -1) perror("File Opening Error\n");
    else {
	dup2(fd_err,1);
    dup2(fd_err,2);
	}
}


void reset(){
    	dup2(dp_in, 0);
		dup2(dp_out, 1);
		dup2(dp_err, 2);
}