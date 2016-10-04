/*
 *         COMP111 Operating Systems
 *                 Fall 2016       
 *                         HW3 Multithreading
 *                                 Mingzhe Li
 *                                 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <assert.h>
#include <ctype.h>


int wordCount(FILE *file) {
	int count = 0;
	int space = 0;
	char c;
	while ((c = fgetc(file)) != EOF) 
		if (isspace(c))
			space = 1;
		else {
			count += space;
			space = 0;
		}

	return count;
}



int main(int argc, char const *argv[])
{
	FILE *f = fopen("Makefile", "r");
	printf("Word Count: %d\n", wordCount(f));
	
	return 0;
}
