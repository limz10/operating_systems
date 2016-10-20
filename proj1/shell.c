/*
	COMP 111 Operating Systems
	Fall 2016
	Project 1 - Basic Shell
	by Mingzhe Li and John Mei Jin Koh
	This program is a basic shell, but not too basic.
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <termcap.h>
#include <errno.h>


// quit
void shell_quit() {
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
	for (int i = 0; arg[i] != NULL; i++) {
		printf("%s", arg[i]);
		// TODO: see TODO

	}
	printf("\n");
}


// this function waits for keyboard input
// and returns whenever ENTER is pressed
void shell_pause() {
	char ENTER = '\n';
	char * keyInput;
	while (keyInput != ENTER)
		scanf(%c, keyInput);
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




int main(int argc, char const *argv[])
{
	// the shell should not exit until quit command is called
	while (1)
	
		// TODO: input stream
		// parse the arguments
		// Do we also want to implement the pipe functionality?






	return 0;
}