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
#include <math.h>


long convertNano(struct timeval t) {
  return (t.tv_sec * 1000000 + t.tv_usec) * 1000;
} 


int return_10() {
	return 10;
} 


int powerOfTen() {
	return pow(10, 1.14);
}


/*
int binaryToDecimal(long n)
{
	int decimal = 0;
	double i = 0;
	int remainder;
	while (n!= 0) {
	        remainder = n % 10;
        	n /= 10;
	        decimal += remainder * pow(2.0, i);
        	++i;
    	}
	
	return decimal;
}
*/


int main(int argc, char const *argv[])
{
	double systemCallTime, procCallTime;
	clock_t startClock, endClock;
	struct timeval start, end;
	long iterations = 1000000;

	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < iterations; i++)
		getpid();
	endClock = gettimeofday(&end, NULL);
//	printf("Start time: %d End time: %d\n", convertNano(start), convertNano(end));
	systemCallTime = convertNano(end) - convertNano(start);
	printf("Avg Time for System Calls: %f\n", systemCallTime/iterations);

	int j;
	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < iterations; i++)
		j = return_10();
	endClock = gettimeofday(&end, NULL);
//	printf("Start time: %d End time: %d\n", convertNano(start), convertNano(end));
        procCallTime = convertNano(end) - convertNano(start);
        printf("Avg Time for Return_10 Calls: %f\n", procCallTime/iterations);

	int k;
	startClock = gettimeofday(&start, NULL);
	for (long i = 0; i < iterations; i++)
		k = powerOfTen();
	endClock = gettimeofday(&end, NULL);
//	printf("Start time: %d End time: %d\n", convertNano(start), convertNano(end));
        procCallTime = convertNano(end) - convertNano(start);
        printf("Avg Time for Power10 Calls: %f\n", procCallTime/iterations);

/*
	int l;
        startClock = gettimeofday(&start, NULL);
        for (long i = 0; i < iterations; i++)
                l = binaryToDecimal(10);
        endClock = gettimeofday(&end, NULL);
        printf("Start time: %d End time: %d\n", convertNano(start), convertNano(end));
        procCallTime = convertNano(end) - convertNano(start);
        printf("Avg Time for BinaryToDecimal Calls: %f\n", procCallTime/iterations);
*/

	return 0;
}
