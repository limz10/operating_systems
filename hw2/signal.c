/*
	COMP111 Operating Systems
	Fall 2016	
	HW2 Signals
	Mingzhe Li
*/

/*
	This program sets up an alarm handler function, in the main()
	the handler was setup by signal(), and alarm clock by alarm()

	After 5 seconds of this program being executed, the alarm clock
	will time out, and the program exits.

	If you manually terminate the program, e.g. by pressing CTRL+C,
	the signal message "SIGNAL #2" will show up, and the program exits.
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void alarm_handler(int signal) {
	printf("SIGNAL #%i!\n", signal);
	exit(1);
}


int main(int argc, char const *argv[])
{
	signal(SIGINT, alarm_handler);
	alarm(5);
	while (1)
		sleep(1);

	return 0;
}