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


// 
void shell_pause(char * keyInput) {

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