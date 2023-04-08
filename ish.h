#ifndef ISH
#define ISH
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <glob.h>
#include <pwd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define PATH 50
#define COMMANDS 10
#define PARAMETERS 10
#define ARGLEN 10
#define JOBS 10

char* hostname();
int getCommand();
void init();
void process();
void executeBuiltin(char* );
int executeCommand(int , char*, char**);
void destroy();

//builtin functions
void saveJob(char*, int );
void printJobs();
void killJob(char*);
void bg(char *);
void fg(char *);
bool isBuiltin(char*);
int cd(char*, int);
void bye();
void printEnv();
void setEnv(char*, char*, int);
void unsetEnv(char*);


//redirection
void ioRedirection(int);
void redirectInput(char*);
void redirectOutput(char*);
void redirectError(char*);
void redirectOutAppend(char*);
void redirectErrAppend(char*);
void reset();
char **inFile;
char **outFile;
char **errFile;
//in out and error redirection
int *redirection[COMMANDS];
int iter_red;
int dp_out , dp_in , dp_err;

// Pipes

int isPipe[COMMANDS-1];
int pipeCount;


// Command and parameters
char ***parametersArray;
char** arglist;
char **commandsArray;

//no of command parameters
int counts[10];
// number of entries in array
int commandCount ;
int parameterCount ;
int paramCount ;

//Jobs

struct Job {
    int jobid;
    int jobpid;
    char *jobstatus;
    char *jobname;  
};

struct Job *jobs;
int jobCount;

extern char **environ;
//background process
bool background;

#endif
