#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <fcntl.h>
#include <signal.h>

void execute(cmdLine *pCmdLine , bool debug){
	if(strcmp(pCmdLine->arguments[0],"quit")==0){
		freeCmdLines(pCmdLine);
		exit(EXIT_SUCCESS);
		
	}else if(strcmp(pCmdLine->arguments[0],"cd")==0){
		if(chdir(pCmdLine->arguments[1])<0){
			perror("cd command fail");
		}
	}else if(strcmp(pCmdLine->arguments[0],"suspend") == 0){
		int pid;
		pid = fork();
		if(pid == 0){
			int suspend_id = atoi(pCmdLine->arguments[1]);
			if(suspend_id<=0){
				fprintf(stderr, "Invalid PID: %s\n",pCmdLine->arguments[1]);
				_exit(EXIT_FAILURE);
			}
			int res = kill(suspend_id,SIGTSTP);
			if(res == -1){
				perror("suspend failed\n");
				_exit(EXIT_FAILURE);
			}
		}
	}else if(strcmp(pCmdLine->arguments[0],"wake") == 0){
		int pid;
		pid = fork();
		if(pid == 0){
			int suspend_id = atoi(pCmdLine->arguments[1]);
			if(suspend_id<=0){
				fprintf(stderr, "Invalid PID: %s\n",pCmdLine->arguments[1]);
				_exit(EXIT_FAILURE);
			}
			int res = kill(suspend_id,SIGCONT);
			if(res == -1){
				perror("wake failed\n");
				_exit(EXIT_FAILURE);
			}
		}	
	}else if(strcmp(pCmdLine->arguments[0],"kill") == 0){
		int pid;
		pid = fork();
		if(pid == 0){
			int suspend_id = atoi(pCmdLine->arguments[1]);
			if(suspend_id<=0){
				fprintf(stderr, "Invalid PID: %s\n",pCmdLine->arguments[1]);
				_exit(EXIT_FAILURE);
			}
			int res = kill(suspend_id,SIGINT);
			if(res == -1){
				perror("kill failed\n");
				_exit(EXIT_FAILURE);
			}
		}		
	}else{
		int pid;
		pid = fork();
		if(pid == 0){
			if(pCmdLine->inputRedirect != NULL){
				int fd = open(pCmdLine->inputRedirect, O_RDONLY);
				if(fd == -1){
					perror("open");
					_exit(EXIT_FAILURE);
				}
				if(dup2(fd, STDIN_FILENO) == -1){
				perror("error dup2");
				_exit(EXIT_FAILURE);
				}
				close(fd);
			}
			
			if(pCmdLine->outputRedirect != NULL){
				int fd = open(pCmdLine->outputRedirect, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
				if(fd == -1){
					perror("open");
					_exit(EXIT_FAILURE);
				}
				if(dup2(fd, STDOUT_FILENO) == -1){
					perror("error dup2");
					_exit(EXIT_FAILURE);
				}
				close(fd);
			}
			if(pCmdLine!=NULL){
			int seccess = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
			if(seccess<0){
				perror("ececution faild");
				_exit(EXIT_FAILURE);
			}
			}
		}
		if(debug){
			fprintf(stderr,"PID: %d\n executing command: %s\n",pid,pCmdLine->arguments[0]);
		}
		if(pid != 0 && pCmdLine->blocking){
			waitpid(pid,NULL,0);
		}
	}
}

int main (int argc, char* argv[]){
	char buffer[2048];
	bool debugMode=false;
	int i;
	for(i=0; i<argc;i++){
		if((strcmp("-d",argv[i])==0)){
			debugMode=true;
		}
	}
	while(1){
	char path_name [PATH_MAX];
	if(getcwd(path_name,PATH_MAX)!=NULL){printf("%s$ ",path_name);}
    if (fgets(buffer,2048,stdin)==NULL){exit(0);}
	cmdLine* line = parseCmdLines(buffer);
	execute(line, debugMode);
	freeCmdLines(line);
	}
	return 0;
}
