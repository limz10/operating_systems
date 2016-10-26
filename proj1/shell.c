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

// some constant values improve readablity
#define MAX_CMD 64
#define BUF_SIZE 1024

extern char **environ;

const char BUF_DELIM[4] = ";\n";
const char SPACE[2] = " ";
const char AMPERSAND[2] = "&";
const char REDIR_REPLACE[2] = ">";
const char REDIR_APPEND[3] = ">>";

FILE *output_file;



/* ------------------------ helper function ------------------------------- */
// helper function
// this function checks if there is whitespace in a string
int is_empty(char *s) {
	while (*s != '\0') {
		if (!isspace(*s))
			return 0;
		s++;
	}

	return 1;
}


// this function removes space in the given string in-place
void remove_space(char* source)
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (*i != ' ')
			i++;
	}
	*i = 0;
}


/* ------------------------ shell function ------------------------------- */
// shell_quit waits for all the children processes to end
// and then exit the program
// note it does not call kill function because it does not expect to 
// force quit any child process
void shell_quit(pid_t *children, int count) {
	for (int i = 0; i < count; i++) {
		// kill(children[i], SIGINT);
		waitpid(children[i], NULL, 0);
	}
	exit(0);
}


// shell_clr utlizes the termcap library to determine
// the terminal type and clears screen by its string capability
void shell_clr() {
	char buffer [BUF_SIZE];
	char *str;

	tgetent(buffer, getenv("TERM"));
	str = tgetstr("cl", NULL);
	fputs(str, stdout);
}


// this function echoes the given string
// it does not support $ variables in this version
void shell_echo(char * arg) {
	printf("%s \n", arg);
}


// shell_pause waits for keyboard input
// and returns whenever ENTER is pressed
void shell_pause() {
	getchar();
}


// shell_cd changes current working directory to
// what is specified in the given string
void shell_cd(char * path) {
	int ret;
	ret = chdir(path);
	if (ret == -1)
		printf("Error: %s\n", strerror(errno));

}


// shell_dir prints out the current working directory
void shell_dir() {
	char * dir = get_current_dir_name();
	printf("%s\n", dir);
}


// shell_help displays the user manual, provided as User Manual
// In interactive mode, it will display a good amount of 
// text based on user's terminal screen size, and prompts
// "press enter" to continue the rest of the manual
void shell_help() {
	FILE *fp = fopen("User Manual", "r");
	char line[80];
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	while (!feof(fp)) {
		for (int j = 0; j < w.ws_row - 1; j++) {
			if (fgets(line, sizeof(line), fp)) {
				printf("%s", line);
			}
		}
		if ((output_file == stdout) && (!feof(fp))) {
			printf("\nPress Enter to continue... \n");
			getchar();
			// removes Press Enter to continue
			printf("\x1b[1A");
			printf("\x1b[2K");
			printf("\x1b[1A");
			printf("\x1b[2K");
			printf("\x1b[1A");
		}

	}
	fclose(fp);
}


// list all the environment strings
void shell_environ() {
	for (char **current = environ; *current; current++) {
	        puts(*current);
	}
}


// check for file redirection
FILE * file_redir(char* cmd_argv, pid_t *children, int index) {
	int arrow_count = 0;
	int double_arrow_count = 0;

	char arrow_buf[BUF_SIZE] = "";
	char output_path[BUF_SIZE] = "";

	// check arrow signs
	// use pointer arithmetic to determine if they are
	// consecutive, and if yes, increment double_arrow_count
	strcpy(arrow_buf, cmd_argv);
	char * double_arrow_ptr = strchr(arrow_buf, '>');
	char * double_arrow_ptr_last = double_arrow_ptr;
	while (double_arrow_ptr != NULL) {
		double_arrow_ptr++;
		double_arrow_ptr = strchr(double_arrow_ptr, '>');
		if((double_arrow_ptr - double_arrow_ptr_last) == 1) {
			double_arrow_count++;
		}
		double_arrow_ptr_last = double_arrow_ptr;
		
		arrow_count++;
	}

	// check for ambiguous output redirect
	// e.g. "a > b >> c" detected
	if ((double_arrow_count > 1) || 
		((arrow_count > 2) && (double_arrow_count == 1)) || 
		((arrow_count > 1) && (double_arrow_count == 0))) {
		fprintf(stderr, 
			"Ambiguous output redirect. \n");
		shell_quit(children, index + 1);
	}


	// if ">>" open file as append
	if (double_arrow_count > 0) {
		double_arrow_ptr = strchr(arrow_buf, '>');
		double_arrow_ptr+=2;
		strcpy(output_path, double_arrow_ptr);

		remove_space(output_path);
		output_file = fopen(output_path, "a");
	}
	// if ">" open file as write
	else if (arrow_count > 0) {
		double_arrow_ptr = strchr(arrow_buf, '>');
		double_arrow_ptr++;
		strcpy(output_path, double_arrow_ptr);
		remove_space(output_path);
		output_file = fopen(output_path, "w");
	}
	// otherwise no file redirection, just print to stdout
	else {
		output_file = stdout;
	}

	return output_file;
}


/* ------------------------ main function ------------------------------- */
int main(int argc, char const *argv[], char** envp)
{
	// set environment values in parent.
	char* myShell;
	myShell = get_current_dir_name();
	strcat(myShell, "/");
	strcat(myShell, argv[0]);
	setenv("SHELL", myShell, 1);


	// determine if the program should have been invoked with 
	// batch mode or interactive mode
	int BATCH_MODE = 0;
	int INTERACTIVE_MODE = 0;
	char buf[1024] = "";
	FILE *batch_file;

	// sanity check
	if (argc > 2) {
		fprintf(stderr, "Too many arguments! \n");
		exit(1);
	}
	else if (argc == 2) {
		// enter batch mode, open batch file for input
		BATCH_MODE = 1;
		batch_file = fopen(argv[1], "r");
	} else {
		// enter interactive mode
		INTERACTIVE_MODE = 1;
	}

	// the shell should not exit until quit command is called
	while (1) {
		// the concurrency list is to record which of the
		// commands should be running concurrently, which
		// is determined by the ";" symbol
		// NOTE: this can be very counterintuitive and is
		// different from the standard linux shell behavior
		int concurrency_list[MAX_CMD];
		int count = 0;
		char * ptr;
		for (int i = 0; i < MAX_CMD ; i++)
			concurrency_list[i] = 0;

		// if interactive mode, read from stdin
		if (INTERACTIVE_MODE == 1) {
			// a small timeout
			// Because the parent process will be running
			// concurrently, in order for the shell prompt
			// to appear after the children processes finish
			// a small timeout is set here.
			// This is intended to make the UI more comfortable
			// in this crazy concurrency.
			usleep(2000);
			printf("basic shell :> ");
			fgets(buf, sizeof(buf), stdin);
			
			// to handle the Ctrl+D input, which sends
			// an EOF from stdin
			if (feof(stdin)) {
				printf("\n");
				exit(0);
			}
			// update the concurrency list
			ptr = buf;
			ptr = strchr(ptr, ';');
			while (ptr != NULL) {
				concurrency_list[count++] = 1;
				ptr++;
				ptr = strchr(ptr, ';');
			}
		}

		// if batch mode, read from batch_file
		else if (BATCH_MODE == 1) {
			char line[BUF_SIZE] = "";
			while (!feof(batch_file)) {
				if (fgets(line, sizeof(line), batch_file)) {
					// print out each line from the 
					// batch file to the user terminal
					// first
					printf("%s", line);
					// update the concurrency list
					ptr = line;
					ptr = strchr(ptr, ';');
					while (ptr != NULL) {
						concurrency_list[count++] = 1;
						ptr++;
						ptr = strchr(ptr, ';');
					}
					strcat(buf, line);
				}
			}

			strcat(buf, "\nquit");
			BATCH_MODE = 0;
		}
		else {
			// this should never be printed
			fprintf(stderr, "RUA! \n"); 
		}

		char cmd_arr[MAX_CMD][BUF_SIZE];
		int cmd_count = 0;

		// our implementation has already run each command
		// in background process, so if an ampersand is read,
		// just make sure the last command is in the concurrency list
		char* tok = strtok(buf, AMPERSAND);
				
		// identify if there should be ";"" or new line
		// BUF_DELIM delimited
		// store the commands into the cmd array and 
		// update the cmd count
		tok = strtok(tok, BUF_DELIM);
		while (tok) {
			strcpy(cmd_arr[cmd_count++], tok);
			tok = strtok(NULL, BUF_DELIM);
		}

		// declare children processess
		pid_t children[cmd_count + 1];

		// execute commands
		for (int i = 0; i < cmd_count; i++) {
			char * cmd = (char*) malloc(BUF_SIZE * sizeof(char));
			assert(cmd);
			char* temp_cmd = strtok(cmd_arr[i], SPACE);
			// if nothing was given, particularly in between
			// two semicolons, proceed with the next command
			if (temp_cmd != NULL) 
				strcpy(cmd, temp_cmd);
			else
				continue;

			// get command argument value if present
			char* cmd_argv = (char*)malloc(BUF_SIZE * sizeof(char));
			assert(cmd_argv);
			char* temp_argv = strtok(NULL, BUF_DELIM);
			if (temp_argv != NULL) {
				strcpy(cmd_argv, temp_argv);
			}

			// quit command has the highest priority
			if (strcmp(cmd, "quit") == 0) {
				shell_quit(children, i + 1);
			}

			// change directory also should run in the 
			// parent process
			if (strcmp(cmd, "cd") == 0) {
				if (is_empty(cmd_argv)) {
					// if directory path not present
					shell_dir();
				}
				else {
					shell_cd(cmd_argv);
				}
				continue;
			}

			children[i] = fork();

			// check fork
			if (children[i] < 0) {
				fprintf(stderr, "Error starting/forking.\n");
			}
			else if (children[i] == 0) {
				// inside child process
				// set child process environment
				char child_env[BUF_SIZE] = "";
				strcpy(child_env, get_current_dir_name());
				strcat(child_env, "/");
				strcat(child_env, argv[0]);
				setenv("PARENT", child_env, 1);

				// check for file redirection
				output_file = file_redir(cmd_argv, children, i);

				// get real argument value before any possible 
				// arrow symbol
				char * arrow_ptr =  strchr(cmd_argv, '>');
				if (arrow_ptr) {
					char temp_argv_no_arrow[BUF_SIZE] = "";
					size_t len = arrow_ptr - cmd_argv;
					strncpy(temp_argv_no_arrow, cmd_argv, len);
					cmd_argv = temp_argv_no_arrow;
				}

				// call the shell functions to
				// execute the commands
				if (strcmp(cmd, "dir") == 0) {
					dup2(fileno(output_file), 1);
					shell_dir();
				}

				else if (strcmp(cmd, "environ") == 0) {
					dup2(fileno(output_file), 1);
					shell_environ();
				}

				else if (strcmp(cmd, "echo") == 0) {
					dup2(fileno(output_file), 1);
					shell_echo(cmd_argv);
				}

				else if (strcmp(cmd, "help") == 0) {
					dup2(fileno(output_file), 1);
					shell_help();
				}

				else if (strcmp(cmd, "pause") == 0) {
					shell_pause();
				}

				else if (strcmp(cmd, "clr") == 0) {
					shell_clr();
				}

				else {
					// not built-in shell functions
					dup2(fileno(output_file), 1);
					if (access(cmd, F_OK) == -1) {
						// invalid cmd_name!
						fprintf(stderr, "Command not found: %s\n", cmd);
						shell_quit(children, i + 1);
					}

					// is valid external program!
					// read program arguments
					int arg_count = 0;
					char ** child_argv = malloc(BUF_SIZE * sizeof(char));
					for (int j = 0; j < BUF_SIZE; j++)
						child_argv[j] = malloc(BUF_SIZE * sizeof(char));
					strcpy(child_argv[0], cmd);
					char* arg_tok = strtok(cmd_argv, SPACE);
					while (arg_tok) {
						strcpy(child_argv[++arg_count], arg_tok);
						arg_tok = strtok(NULL, SPACE);
					}
					child_argv[++arg_count] = NULL;
					char* const child_envp[] = {NULL};
					// execute the program
					execve(cmd, child_argv, child_envp);
					// emulates invalid program invocation case in
					// linux bash.
					// Only activates when shell tries to execute
					// invalid program
					fprintf(stderr, "Command not found: %s\n", cmd);
				}
				// exit the child process
				exit(0);
			}

			else if (concurrency_list[i] == 0) {
				waitpid(children[i], NULL, 0);
			}
		}
	}

	return 0;
}