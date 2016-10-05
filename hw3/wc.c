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
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <ctype.h>


int wordCount(char* filePath) {
	FILE *file = fopen(filePath, "r");
	if (file == NULL) {
		fprintf (stderr, "file does not exist\n");
		exit (1); 
	}

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

	fclose(file);
	return count;
}



int main(int argc, char const *argv[])
{
	if (argc < 2) {
		fprintf (stderr, "usage: ./wc FILE [FILE...]\n");
		exit (1); 
	}

	pthread_t * thread_ids;
	thread_ids = calloc(argc, sizeof(pthread_t));
	int rtn = 0;
	int totalCount = 0;

	for (int i = 1; i < argc; i++) {
		if (pthread_create(thread_ids+i, NULL, (void*)wordCount, (void*)argv[i]) == 0)
		{
			pthread_join(*(thread_ids+i), (void *)&rtn);
			printf("Word Count: %d in %s\n", rtn, argv[i]);

// fprintf(stderr, "ttc = %d\n", totalCount);
int ftmdx = rtn;
totalCount += ftmdx;
// fprintf(stderr, "tc = %d\n", totalCount);

		} 
		else {
			printf("Could not create thread!\n");
		}
	
		//int count = wordCount(argv[i]);
		//printf("Word Count: %d in %s\n", count, argv[i]);
	}

	printf("Total Word Count: %d\n", totalCount);
	free(thread_ids);
	return 0;
}
