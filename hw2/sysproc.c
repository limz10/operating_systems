/*
	COMP111 Operating Systems
	Fall 2016	
	HW2 System Calls Versus Procedure Calls
	Mingzhe Li
*/


#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>  
#include <sys/times.h>   
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <limits.h> 
#include <time.h>
#include <signal.h>


int return_10() {
	return 10;
} 

int main(int argc, char const *argv[])
{
	double systemCallTime, procCallTime;
	struct timeval start, end;
	clock_t startClock, endClock;

	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < 1000; i++)
		getpid();
	endClock = gettimeofday(&end, NULL);

	systemCallTime = (double) (endClock - startClock) / CLOCKS_PER_SEC;

	int j;
	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < 1000; i++)
		j = return_10();
	endClock = gettimeofday(&end, NULL);

	procCallTime = (double) (endClock - startClock) / CLOCKS_PER_SEC;


	printf("Time for System Calls: %f\n", systemCallTime);
	printf("Time for Procedure Calls: %f\n", procCallTime);


	return 0;
}