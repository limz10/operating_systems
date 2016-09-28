/*
	COMP111 Operating Systems
	Fall 2016	
	HW2 System Calls Versus Procedure Calls
	Mingzhe Li
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


int return_10() {
	return 10;
} 

int main(int argc, char const *argv[])
{
	double systemCallTime, procCallTime;
	struct tms start, end;
	clock_t start_clock, end_clock;

	gettimeofday(&start);
	for (long i = 0; i < 1000; i++)
		getpid();
	gettimeofday(&end);

	systemCallTime = (double) (end_clock - start_clock) / CLOCKS_PER_SEC;

	gettimeofday(&start);
	for (long i = 0; i < 1000; i++)
		int j = return_10();
	gettimeofday(&end);

	procCallTime = (double) (end_clock - start_clock) / CLOCKS_PER_SEC;


	printf("Time for System Calls: %f\n", systemCallTime);
	printf("Time for Procedure Calls: %f\n", procCallTime);


	return 0;
}