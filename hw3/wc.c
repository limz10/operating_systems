/*
	COMP111 Operating Systems
	Fall 2016       
 	HW3 Multithreading
 	Mingzhe Li
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <assert.h>
#include <ctype.h>


void wordCount(FILE *file) {
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

	printf("Word Count: %d in %s\n", count, file);
}



int main(int argc, char const *argv[])
{
	if (argc < 2) {
		fprintf (stderr, "usage: ./wc FILE [FILE...]\n");
		exit (1); 
	}

	pthread_t * thread_ids;
	thread_ids = calloc(argc, sizeof(pthread_t));

	for (int i = 1; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		if (pthread_create(thread_ids+i, NULL, wordCount, f) == 0)
		{
			pthread_join(thread_ids+i, (void *)&rtn);
			printf("Successfully returned thread with value %d\n", rtn);
		} 
		else {
			printf("Could not create thread!\n");
		}


		free(thread_ids);
		return 0;
	}
}
