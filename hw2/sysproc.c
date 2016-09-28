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


long convertNano(struct timeval t) {
  return (t.tv_sec * 1000000 + t.tv_usec) * 1000;
} 


int return_10() {
	return 10;
} 

char* memAlloc() {
	char* string = (char*)malloc(10);
	return string;
}


int main(int argc, char const *argv[])
{
	double systemCallTime, procCallTime;
	clock_t startClock, endClock;
	struct timeval start, end;

	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < 1000000000; i++)
		getpid();
	endClock = gettimeofday(&end, NULL);
	printf("Start time: %d End time: %d\n", (double)startClock, (double)endClock);
	systemCallTime = convertNano(end) - convertNano(start);
	printf("Time for System Calls: %d\n", systemCallTime);

	int j;
	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < 1000000000; i++)
		j = return_10();
	endClock = gettimeofday(&end, NULL);
	printf("Start time: %d End time: %d\n", (double)startClock, (double)endClock);
	procCallTime = convertNano(end) - convertNano(start);
	printf("Time for return_10 Calls: %d\n", procCallTime);


	char* j;
	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < 1000000000; i++)
		j = memAlloc();
	endClock = gettimeofday(&end, NULL);
	printf("Start time: %d End time: %d\n", (double)startClock, (double)endClock);
	procCallTime = convertNano(end) - convertNano(start);
	printf("Time for memAlloc Calls: %d\n", procCallTime);


	return 0;
}