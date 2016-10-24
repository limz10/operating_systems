/*
	COMP 111 Operating Systems
	Fall 2016
	Project 1 - Basic Shell
	by Mingzhe Li and John Mei Jin Koh
	This program is a basic shell, but not too basic.
*/ 

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <termcap.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>


const char BUF_DELIM[4] = ";\n";
const char SPACE[2] = " ";


// helper function
int is_empty(char *s) {
	while (*s != '\0') {
		if (!isspace(*s))
			return 0;
		s++;
	}
	return 1;
}


// quit
void shell_quit(pid_t *children, int count) {
	for (int i = 0; i < count; i++) {
		kill(children[i], SIGINT);
	}
	exit(0);
}


// this function utlizes the termcap library to determine 
// the terminal type and clears screen by its string capability
void shell_clr() {
	char buffer [1024];
	char *str;

	tgetent(buffer, getenv("TERM"));
	str = tgetstr("cl", NULL);
	fputs(str, stdout);
} 


// TODO: does this function needs to deal with signs to expand 
//		 argument variables
// 		 ?
void shell_echo(char * arg) {
	printf("%s \n", arg);
}


// this function waits for keyboard input
// and returns whenever ENTER is pressed
void shell_pause() {
	char ENTER = '\n';
	char keyInput = 'a';
	while (keyInput != ENTER) {
		scanf("%c", keyInput);
	}
}


// 
void shell_cd(char * path) {
	int ret;
	ret = chdir(path);
	if (ret == -1)
		printf("Error: %s\n", strerror(errno));

}


// this function prints out the current directory
void shell_dir() {
	char * dir = get_current_dir_name();
	printf("%s\n", dir);
}


// display the user manual
void shell_help() {
	FILE *fp = fopen("User Manual", "r");
	char line[80];
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	printf("ROW %i\n", w.ws_row);
	while (!feof(fp)) {
		for (int j = 0; j < w.ws_row - 1; j++) {
			if (fgets(line, sizeof(line), fp)) {
				printf("%s", line); 
			}
		}
		printf("\nPress Enter to continue... \n");
		getchar();	
	}
	fclose(fp);
}	

	


// list all the environment strings
void shell_environ(char** envp) {
	char** env;
	for (env = envp; *env != 0; env++) {
		char* thisEnv = *env;
		printf("%s\n", thisEnv);
	}
}


int main(int argc, char const *argv[], char** envp)
{
	// the shell should not exit until quit command is called
	if(argc >= 2) {
		// command inputted via execution!
		// argv[]
	}

	while (1) {
		printf("shell :> ");
		char buf[1024];
		fgets(buf, sizeof(buf), stdin);
		// max cmd's in the arr, 128.
		char *cmd_arr[32][1024];
		// count of {cmd [argv]} token sets
		int cmd_count = 0;
		
		// first determine if there's ;'s 
		// BUF_DELIM delimited
		char* tok = strtok(buf, BUF_DELIM);

		while (tok) {
			strcpy(cmd_arr[cmd_count++], tok);
			tok = strtok(NULL, BUF_DELIM);
			
		}

		pid_t children[cmd_count+1];

		// execute cmd
		for (int i = 0; i < cmd_count; i++) {			
			char * cmd = (char*) malloc(1024 * sizeof(char));
			char* temp = strtok(cmd_arr[i], SPACE);				
			strcpy(cmd, temp);

			char * cmd_argv = (char*) malloc(1024 * sizeof(char));
			char* temp_argv = strtok(NULL, BUF_DELIM);
			if (temp_argv != NULL) {
				strcpy(cmd_argv, temp_argv);
			}

			// quit has the highest priority
			if (strcmp(cmd, "quit") == 0) {
				shell_quit(children, i + 1);
			}

			children[i] = fork();

			if (children[i] < 0) {
				fprintf(stderr, "Error starting/forking.\n");
			} 

			else if (children[i] == 0) {
				if (strcmp(cmd, "cd") == 0) {
					if (is_empty(cmd_argv))
						shell_dir();
					else
						shell_cd(cmd_argv);
				}
				else if (strcmp(cmd, "dir") == 0) {
					shell_dir();
				}
				
				else if (strcmp(cmd, "environ") == 0) {
					shell_environ(envp);
				}
				
				else if (strcmp(cmd, "echo") == 0) {
					shell_echo(cmd_argv);
				}
				
				else if (strcmp(cmd, "help") == 0) {
					shell_help();
				}
				
				else if (strcmp(cmd, "pause") == 0) {
					shell_pause();
				}
				
				else if (strcmp(cmd, "clr") == 0) {
					shell_clr();
				}
				
				else {
                                        if (access(cmd, F_OK) == -1) {
                                                // invalid cmd_name!
                                                fprintf(stderr, "Command not found %s\n", cmd);
                                                return -1;
                                        }
                                        // is valid external proggy!
                                        char* const childArgv[] = {cmd_argv, NULL};
                                        char* const childEnvp[] = {NULL};
                                        
                                        // execute the program
                                        execve(cmd, childArgv, childEnvp);
                                        // return not expected, just for the sake of sanity check.
                                        printf("Return not expected. Something is wrong.\n");
                                }
			} 

			else {
				waitpid(children[i], NULL, 0);
				printf("WAITING\n");
			}
		}
	}
	
		// TODO: input stream
		// parse the arguments
		// Do we also want to implement the pipe functionality?


	return 0;
}