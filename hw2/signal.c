/*
	COMP111 Operating Systems
	Fall 2016	
	HW2 Signals
	Mingzhe Li
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <alarm.h>



void alarm_handler(int signal) {
	printf("ALARM #%i!\n", signal);
	exit(1);
}

int main(int argc, char const *argv[])
{
	signal(10, alarm_handler);
	alarm(5);

	return 0;
}